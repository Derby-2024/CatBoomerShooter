// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDirectorGameMode.h"
#include "../TeamsProjectSettings.h"
#include "Kismet/KismetSystemLibrary.h"

//FEnemyToken AAIDirectorGameMode::RequestToken(ETokenType TokenType, AAIEnemyBaseController* EnemyController, AActor* TargetActor)
void AAIDirectorGameMode::RequestToken(AAIEnemyBaseController* EnemyController, const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, UEnemyToken*& Token, bool& Success)
{
	if (!IsValid(EnemyController))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIDirectorGameMode::RequestToken: Invalid Enemy controller provided."));
		return;
	}
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIDirectorGameMode::RequestToken: Invalid target actor provided."));
		return;
	}

	// Get storage for target actor
	FActorTokensCollection* TargetTokens = ActorTokens.Find(TargetActor);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIDirectorGameMode::RequestToken: Target actor doesn't have any associated tokens."));
		return;
	}

	// Get storage of token type for target actor
	FTokenCollection* TokenCollection = TargetTokens->GetCollectionOfType(TokenType);

	// There is a valid token that can be claimed
	if (TokenCollection->FreeTokens.Num() > 0)
	{
		UEnemyToken* ClaimedToken = TokenCollection->FreeTokens.Pop();
		ClaimedToken->ClaimedBy = EnemyController;

		TokenCollection->ClaimedTokens.Add(ClaimedToken);

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AAIDirectorGameMode::TokenTimeout, ClaimedToken);
		// Do we really need to check if timer manager is valid???
		GetWorldTimerManager().SetTimer(ClaimedToken->TimerHandle, TimerDelegate, TOKEN_TIMEOUT, false);

		UE_LOG(
			LogTemp, Log, 
			TEXT("AIDirectorGameMode::RequestToken: Enemy %s claimed token %s of type %s and priority %s against actor %s."),
			*UKismetSystemLibrary::GetDisplayName(EnemyController), 
			*UKismetSystemLibrary::GetDisplayName(ClaimedToken),
			*UEnum::GetValueAsString(TokenType),
			*UEnum::GetValueAsString(TokenPriority),
			*UKismetSystemLibrary::GetDisplayName(TargetActor)
		);

		Success = true;
		Token = ClaimedToken;
	}
}

void AAIDirectorGameMode::ReleaseToken(UEnemyToken* ReleasedToken, const float CustomCooldown)
{
	if (ReleasedToken == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::ReleaseToken: nullptr provided for token."));
		return;
	}

	// Get storage for token owner
	FActorTokensCollection* TargetTokens = ActorTokens.Find(ReleasedToken->Owner);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::ReleaseToken: Released token doesn't have a valid owner."));
		return;
	}

	// Get storege of token type for target actor
	FTokenCollection* TokenCollection = TargetTokens->GetCollectionOfType(ReleasedToken->TokenType);

	if (TokenCollection->ClaimedTokens.Remove(ReleasedToken) == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::ReleaseToken: Could not find claimed token to release."));
		return;
	}

	UE_LOG(
		LogTemp, Log,
		TEXT("AIDirectorGameMode::ReleaseToken: Enemy %s released a token against actor %s."),
		*UKismetSystemLibrary::GetDisplayName(ReleasedToken->ClaimedBy),
		*UKismetSystemLibrary::GetDisplayName(ReleasedToken->Owner)
	);

	// Stop token timeout
	GetWorldTimerManager().ClearTimer(ReleasedToken->TimerHandle);

	float Cooldown = CustomCooldown < 0.0f ? TOKEN_COOLDOWN : CustomCooldown;

	// We want the token to go straight back to the pool
	if (Cooldown == 0)
	{
		ReleasedToken->ClaimedBy = nullptr;
		TokenCollection->FreeTokens.Add(ReleasedToken);
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("AIDirectorGameMode::ReleaseToken: Token on cooldown for %f seconds."), Cooldown);

	// We want the token to be temporarily unavailable
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AAIDirectorGameMode::TokenCooldownEnd, ReleasedToken);
	// Do we really need to check if timer manager is valid???
	GetWorldTimerManager().SetTimer(ReleasedToken->TimerHandle, TimerDelegate, Cooldown, false);

	TokenCollection->CooldownTokens.Add(ReleasedToken);
}

void AAIDirectorGameMode::TokenTimeout(UEnemyToken* Token)
{
	if (Token == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AIDirectorGameMode::TokenTimeout: Token is invalid"));
		return;
	}

	UE_LOG(
		LogTemp, Warning,
		TEXT("AIDirectorGameMode::TokenTimeout: Token %s (owned by %s) has timed out while being held by %s."),
		*UKismetSystemLibrary::GetDisplayName(Token),
		*UKismetSystemLibrary::GetDisplayName(Token->Owner),
		*UKismetSystemLibrary::GetDisplayName(Token->ClaimedBy)
	);

	ReleaseToken(Token);
}

void AAIDirectorGameMode::TokenCooldownEnd(UEnemyToken* Token)
{
	if (Token == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AIDirectorGameMode::TokenCooldownEnd: Token is invalid"));
		return;
	}

	// Get storage for token owner
	FActorTokensCollection* TargetTokens = ActorTokens.Find(Token->Owner);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::TokenCooldownEnd: Released token doesn't have a valid owner."));
		return;
	}

	// Get storege of token type for target actor
	FTokenCollection* TokenCollection = TargetTokens->GetCollectionOfType(Token->TokenType);

	if (TokenCollection->CooldownTokens.Remove(Token) == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::TokenCooldownEnd: Token was not marked as on cooldown."));
		return;
	}

	UE_LOG(
		LogTemp, Log,
		TEXT("AIDirectorGameMode::TokenCooldownEnd: Token %s (owned by %s) has finished cooldown after being held by %s."),
		*UKismetSystemLibrary::GetDisplayName(Token),
		*UKismetSystemLibrary::GetDisplayName(Token->Owner),
		*UKismetSystemLibrary::GetDisplayName(Token->ClaimedBy)
	);

	Token->ClaimedBy = nullptr;
	TokenCollection->FreeTokens.Add(Token);
}

void AAIDirectorGameMode::AddTokensToActor(AActor* TargetActor, ETokenType TokenType, int Amount)
{
	// Only accept positive number of tokens
	if (Amount <= 0)
	{
		UE_LOG(
			LogTemp, Warning, 
			TEXT("AIDirectorGameMode::AddTokensToActor: Invalid number of tokens (%d) requested to add to actor %s."),
			Amount, *UKismetSystemLibrary::GetDisplayName(TargetActor)
		);
		return;
	}
	
	// Check if actor is pending destroy or pointer invalid
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIDirectorGameMode::AddTokensToActor: Invalid target actor provided."));
		return;
	}

	FActorTokensCollection* TargetTokens = ActorTokens.Find(TargetActor);

	if (TargetTokens == nullptr)
	{
		// Actor Tokens don't exist. Create a new collection and store it.
		FActorTokensCollection NewActorTokens;
		ActorTokens.Add(TargetActor, NewActorTokens);
		// Get pointer to new tokens
		TargetTokens = ActorTokens.Find(TargetActor);

		UE_LOG(
			LogTemp, Log,
			TEXT("AIDirectorGameMode::AddTokensToActor: Added new actor (%s) to ActorTokens."),
			*UKismetSystemLibrary::GetDisplayName(TargetActor)
		);
	}

	// Get the collection for tokens of requested type
	FTokenCollection* TokenCollection = TargetTokens->GetCollectionOfType(TokenType);

	for (int i = 0; i < Amount; i++)
	{
		// Create new token to add to token array
		UEnemyToken* NewToken = NewObject<UEnemyToken>();
		NewToken->Owner = TargetActor;
		NewToken->TokenType = TokenType;

		// Add "amount" number of tokens
		TokenCollection->FreeTokens.Add(NewToken);
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("AIDirectorGameMode::AddTokensToActor: Added %d tokens of type %s to actor %s"),
		Amount, *UEnum::GetValueAsString(TokenType), *UKismetSystemLibrary::GetDisplayName(TargetActor)
	);
}

void AAIDirectorGameMode::AddDefaultTokensToActor(AActor* TargetActor) 
{
	// Check if actor is pending destroy or pointer invalid
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIDirectorGameMode::AddDefaultTokensToActor: Invalid target actor provided."));
		return;
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("AIDirectorGameMode::AddDefaultTokensToActor: Adding default tokens to actor %s"),
		*UKismetSystemLibrary::GetDisplayName(TargetActor)
	);

	AddTokensToActor(TargetActor, ETokenType::Normal, 3);
	AddTokensToActor(TargetActor, ETokenType::Heavy, 1);
}

void AAIDirectorGameMode::StartPlay()
{
	FGenericTeamId::SetAttitudeSolver(&UTeamsProjectSettings::GetAttitude);
	UE_LOG(LogTemp, Log, TEXT("AIDirectorGameMode::StartPlay: Start Play"));

	Super::StartPlay();
}