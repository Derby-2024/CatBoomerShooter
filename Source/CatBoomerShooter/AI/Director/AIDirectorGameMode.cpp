// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDirectorGameMode.h"
#include "../TeamsProjectSettings.h"
#include "../AIEnemyBaseController.h"
#include "../EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTokenSystem);
#define DEFAULT_LOG_LEVEL Verbose

void AAIDirectorGameMode::CheckPlayers()
{
	for (FPlayerData& Data : PlayerData)
	{
		Data.UpdatePositionData();

		FVector Pos = Data.PlayerPawn->GetActorLocation() + (Data.TotalMovement / 3.f);
		//FVector Pos = Data.TotalMovement;

		DrawDebugBox(GetWorld(), Pos, FVector(10., 10., 10.), FColor::Red, false, 5.0f);
	}
}

void AAIDirectorGameMode::RegisterPlayer(APawn* PlayerPawn)
{
	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::RegisterPlayer: Invalid player pawn provided."));
		return;
	}

	UE_LOG(
		LogTemp, Verbose,
		TEXT("Registering new Player: %s"),
		*UKismetSystemLibrary::GetDisplayName(PlayerPawn)
	);

	FPlayerData NewPlayerData = FPlayerData(PlayerPawn);
	PlayerData.Add(NewPlayerData);
}

void AAIDirectorGameMode::RemovePlayer(APawn* PlayerPawn)
{
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::RemovePlayer: nullptr provided."));
		return;
	}

	// Iterates through player data until finds the correct one for the player.
	// Not sure that I like this. Might change to something simpler at a later date.
	for (int n = 0; n < PlayerData.Num(); n++)
	{
		if (PlayerData[n].PlayerPawn == PlayerPawn)
		{
			PlayerData.RemoveAt(n);

			UE_LOG(
				LogTemp, Verbose,
				TEXT("Removing Player: %s"),
				*UKismetSystemLibrary::GetDisplayName(PlayerPawn)
			);

			break;
		}
	}
}

void AAIDirectorGameMode::RegisterEnemy(AActor* EnemyActor)
{
	// Check that Enemy Actor isn't null or pending removal
	if (!IsValid(EnemyActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::RegisterEnemy: Invalid enemy actor provided."));
		return;
	}

	// Check that Enemy Actor implements enemy interface
	IEnemyBase* Enemy = Cast<IEnemyBase>(EnemyActor);
	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::RegisterEnemy:: Provided actor doesn't implement enemy interface."));
		return;
	}

	UE_LOG(
		LogTemp, Verbose, 
		TEXT("Registering new enemy: %d %s"),
		EnemyActor->GetUniqueID(),
		*UKismetSystemLibrary::GetDisplayName(EnemyActor)
	);

	// Add enemy to general array and enum type specific array
	Enemies.Enemies.Add(EnemyActor);
	Enemies.GetCollectionOfType(IEnemyBase::Execute_GetEnemyType(EnemyActor))->Add(EnemyActor);
}

void AAIDirectorGameMode::RemoveEnemy(AActor* EnemyActor)
{
	if (EnemyActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIDirectorGameMode::RemoveEnemy: nullptr provided."));
		return;
	}

	UE_LOG(
		LogTemp, Verbose,
		TEXT("Removing enemy: %d %s"),
		EnemyActor->GetUniqueID(),
		*UKismetSystemLibrary::GetDisplayName(EnemyActor)
	);

	// Remove enemy from general storage
	Enemies.Enemies.Remove(EnemyActor);

	// Confirm enemy implements enemy interface
	IEnemyBase* Enemy = Cast<IEnemyBase>(EnemyActor);
	if (Enemy == nullptr)
	{
		UE_LOG(
			LogTemp, Warning, 
			TEXT("AAIDirectorGameMode::RemoveEnemy:: Provided actor %s doesn't implement enemy interface."), 
			*UKismetSystemLibrary::GetDisplayName(EnemyActor)
		);
		return;
	}

	// Remove enemy from type specific storage
	Enemies.GetCollectionOfType(IEnemyBase::Execute_GetEnemyType(EnemyActor))->Remove(EnemyActor);
}

void AAIDirectorGameMode::GetEnemyActors(TArray<AActor*>& EnemyActors)
{
	EnemyActors = Enemies.Enemies;
}

void AAIDirectorGameMode::GetEnemyActorsTyped(const EEnemyType EnemyType, TArray<AActor*>& EnemyActors)
{
	EnemyActors = *Enemies.GetCollectionOfType(EnemyType);
}

void AAIDirectorGameMode::GetEnemyActorsInRange(const FVector Origin, const float MinRadius, const float MaxRadius, TArray<AActor*>& EnemyActors)
{
	for (AActor*& Enemy : Enemies.Enemies)
	{
		float Distance = FVector::Distance(Origin, Enemy->GetActorLocation());

		if (MinRadius <= Distance && Distance < MaxRadius)
		{
			EnemyActors.Add(Enemy);
		}
	}
}

void AAIDirectorGameMode::GetEnemyActorsInRangeTyped(const FVector Origin, const float MinRadius, const float MaxRadius, const EEnemyType EnemyType, TArray<AActor*>& EnemyActors)
{
	for (AActor*& Enemy : *Enemies.GetCollectionOfType(EnemyType))
	{
		float Distance = FVector::Distance(Origin, Enemy->GetActorLocation());

		if (MinRadius <= Distance && Distance < MaxRadius)
		{
			EnemyActors.Add(Enemy);
		}
	}
}


//FEnemyToken AAIDirectorGameMode::RequestToken(ETokenType TokenType, AAIEnemyBaseController* EnemyController, AActor* TargetActor)
void AAIDirectorGameMode::RequestToken(AAIEnemyBaseController* EnemyController, const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, UEnemyToken*& Token, bool& Success)
{
	if (!IsValid(EnemyController))
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::RequestToken: Invalid Enemy controller provided."));
		return;
	}
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::RequestToken: Invalid target actor provided."));
		return;
	}

	// Get storage for target actor
	FActorTokensCollection* TargetTokens = ActorTokens.Find(TargetActor);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::RequestToken: Target actor doesn't have any associated tokens."));
		return;
	}

	// Don't allow tokens to be taken out if locked
	if (TargetTokens->Locked) return;

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
			LogTokenSystem, DEFAULT_LOG_LEVEL,
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

	if (TokenPriority == ETokenPriority::High)
	{
		UEnemyToken* TokenToTake = nullptr;
		bool Found = false;

		for (UEnemyToken*& ClaimedToken : TokenCollection->ClaimedTokens)
		{
			if (ClaimedToken->ClaimPriority == ETokenPriority::Low)
			{
				TokenToTake = ClaimedToken;
				Found = true;
				break;
			}
		}

		if (Found)
		{
			AAIEnemyBaseController* PreviousOwner = TokenToTake->ClaimedBy;

			PreviousOwner->TokenRetracted(TokenToTake);

			TokenToTake->ClaimedBy = EnemyController;
			TokenToTake->ClaimPriority = ETokenPriority::High;

			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &AAIDirectorGameMode::TokenTimeout, TokenToTake);
			// Do we really need to check if timer manager is valid???
			GetWorldTimerManager().SetTimer(TokenToTake->TimerHandle, TimerDelegate, TOKEN_TIMEOUT, false);

			UE_LOG(
				LogTokenSystem, DEFAULT_LOG_LEVEL,
				TEXT("AIDirectorGameMode::RequestToken: Enemy %s took token %s of type %s from %s."),
				*UKismetSystemLibrary::GetDisplayName(EnemyController),
				*UKismetSystemLibrary::GetDisplayName(TokenToTake),
				*UEnum::GetValueAsString(TokenType),
				*UKismetSystemLibrary::GetDisplayName(PreviousOwner)
			);

			Success = true;
			Token = TokenToTake;
		}
	}
}

void AAIDirectorGameMode::ReleaseToken(UEnemyToken* ReleasedToken, const float CustomCooldown)
{
	if (ReleasedToken == nullptr)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AAIDirectorGameMode::ReleaseToken: nullptr provided for token."));
		return;
	}

	// Get storage for token owner
	FActorTokensCollection* TargetTokens = ActorTokens.Find(ReleasedToken->Owner);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AAIDirectorGameMode::ReleaseToken: Released token doesn't have a valid owner."));
		return;
	}

	// Get storege of token type for target actor
	FTokenCollection* TokenCollection = TargetTokens->GetCollectionOfType(ReleasedToken->TokenType);

	if (TokenCollection->ClaimedTokens.Remove(ReleasedToken) == 0)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AAIDirectorGameMode::ReleaseToken: Could not find claimed token to release."));
		return;
	}

	UE_LOG(
		LogTokenSystem, DEFAULT_LOG_LEVEL,
		TEXT("AIDirectorGameMode::ReleaseToken: Enemy %s released a token against actor %s."),
		*UKismetSystemLibrary::GetDisplayName(ReleasedToken->ClaimedBy),
		*UKismetSystemLibrary::GetDisplayName(ReleasedToken->Owner)
	);

	// Stop token timeout
	GetWorldTimerManager().ClearTimer(ReleasedToken->TimerHandle);

	float Cooldown = CustomCooldown < 0.0f ? TOKEN_COOLDOWN : CustomCooldown;

	// We want the token to go straight back to the pool
	// Reset it's values to default then return it to free tokens
	if (Cooldown == 0)
	{
		ReleasedToken->ClaimedBy = nullptr;
		ReleasedToken->ClaimPriority = ETokenPriority::Low;
		TokenCollection->FreeTokens.Add(ReleasedToken);
		return;
	}

	//UE_LOG(LogTokenSystem, Verbose, TEXT("AIDirectorGameMode::ReleaseToken: Token on cooldown for %f seconds."), Cooldown);

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
		UE_LOG(LogTokenSystem, Error, TEXT("AIDirectorGameMode::TokenTimeout: Token is invalid"));
		return;
	}

	UE_LOG(
		LogTokenSystem, Warning,
		TEXT("AIDirectorGameMode::TokenTimeout: Token %s (owned by %s) has timed out while being held by %s."),
		*UKismetSystemLibrary::GetDisplayName(Token),
		*UKismetSystemLibrary::GetDisplayName(Token->Owner),
		*UKismetSystemLibrary::GetDisplayName(Token->ClaimedBy)
	);

	//Token->ClaimedBy->TokenRetracted(Token);
	ReleaseToken(Token);
}

void AAIDirectorGameMode::TokenCooldownEnd(UEnemyToken* Token)
{
	if (Token == nullptr)
	{
		UE_LOG(LogTokenSystem, Error, TEXT("AIDirectorGameMode::TokenCooldownEnd: Token is invalid"));
		return;
	}

	// Get storage for token owner
	FActorTokensCollection* TargetTokens = ActorTokens.Find(Token->Owner);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AAIDirectorGameMode::TokenCooldownEnd: Released token doesn't have a valid owner."));
		return;
	}

	// Get storege of token type for target actor
	FTokenCollection* TokenCollection = TargetTokens->GetCollectionOfType(Token->TokenType);

	if (TokenCollection->CooldownTokens.Remove(Token) == 0)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AAIDirectorGameMode::TokenCooldownEnd: Token was not marked as on cooldown."));
		return;
	}

	UE_LOG(
		LogTokenSystem, DEFAULT_LOG_LEVEL,
		TEXT("AIDirectorGameMode::TokenCooldownEnd: Token %s (owned by %s) has finished cooldown after being held by %s."),
		*UKismetSystemLibrary::GetDisplayName(Token),
		*UKismetSystemLibrary::GetDisplayName(Token->Owner),
		*UKismetSystemLibrary::GetDisplayName(Token->ClaimedBy)
	);

	Token->ClaimedBy = nullptr;
	Token->ClaimPriority = ETokenPriority::Low;
	TokenCollection->FreeTokens.Add(Token);
}

void AAIDirectorGameMode::AddTokensToActor(AActor* TargetActor, ETokenType TokenType, int Amount)
{
	// Only accept positive number of tokens
	if (Amount <= 0)
	{
		UE_LOG(
			LogTokenSystem, Warning, 
			TEXT("AIDirectorGameMode::AddTokensToActor: Invalid number of tokens (%d) requested to add to actor %s."),
			Amount, *UKismetSystemLibrary::GetDisplayName(TargetActor)
		);
		return;
	}
	
	// Check if actor is pending destroy or pointer invalid
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::AddTokensToActor: Invalid target actor provided."));
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
			LogTokenSystem, Log,
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
		LogTokenSystem, DEFAULT_LOG_LEVEL,
		TEXT("AIDirectorGameMode::AddTokensToActor: Added %d tokens of type %s to actor %s"),
		Amount, *UEnum::GetValueAsString(TokenType), *UKismetSystemLibrary::GetDisplayName(TargetActor)
	);
}

void AAIDirectorGameMode::AddDefaultTokensToActor(AActor* TargetActor) 
{
	// Check if actor is pending destroy or pointer invalid
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::AddDefaultTokensToActor: Invalid target actor provided."));
		return;
	}

	UE_LOG(
		LogTokenSystem, Log,
		TEXT("AIDirectorGameMode::AddDefaultTokensToActor: Adding default tokens to actor %s"),
		*UKismetSystemLibrary::GetDisplayName(TargetActor)
	);

	AddTokensToActor(TargetActor, ETokenType::Normal, 3);
	AddTokensToActor(TargetActor, ETokenType::Heavy, 1);
}

void AAIDirectorGameMode::SetTokensLocked(AActor* TargetActor, bool Locked)
{
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::SetTokensLocked: Invalid target actor provided."));
		return;
	}

	// Get storage for target actor
	FActorTokensCollection* TargetTokens = ActorTokens.Find(TargetActor);

	if (TargetTokens == nullptr)
	{
		UE_LOG(LogTokenSystem, Warning, TEXT("AIDirectorGameMode::SetTokensLocked: Target actor doesn't have any associated tokens."));
		return;
	}

	UE_LOG(
		LogTokenSystem, Log, 
		TEXT("AIDirectorGameMode::SetTokensLocked: Set tokens locked state for target %s to %s."),
		*UKismetSystemLibrary::GetDisplayName(TargetActor),
		Locked ? TEXT("true") : TEXT("false")
	);

	TargetTokens->Locked = Locked;
}

void AAIDirectorGameMode::StartPlay()
{
	UE_LOG(LogTemp, Log, TEXT("AIDirectorGameMode::StartPlay: Start Play"));
	FGenericTeamId::SetAttitudeSolver(&UTeamsProjectSettings::GetAttitude);

	// Start timer to check players 
	GetWorldTimerManager().SetTimer(PlayerCheckTimerHandle, this, &AAIDirectorGameMode::CheckPlayers, PLAYER_CHECK_INTERVAL, true);

	Super::StartPlay();
}