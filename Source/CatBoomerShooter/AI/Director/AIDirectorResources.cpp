// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDirectorResources.h"
#include "../EnemyBase.h"

FPlayerData::FPlayerData()
{
	PlayerDistances.InsertZeroed(0, PLAYER_MOVEMENT_RECORD_SIZE);
	PlayerMovementIndex = 0;
	PlayerCampingIndex = 0;
}

TArray<AActor*>* FEnemyCollection::GetCollectionOfType(EEnemyType EnemyType)
{
	switch (EnemyType)
	{
		case EEnemyType::Swarm:
			return &SwarmEnemies;
		case EEnemyType::Ranged:
			return &RangedEnemies;
		case EEnemyType::Tank:
			return &TankEnemies;
		case EEnemyType::Boss:
			return &BossEnemies;
		default:
			UE_LOG(LogTemp, Error, TEXT("FActorTokensCollection::GetCollectionOfType: Invalid Token Type provided."));
			return nullptr;
	}
}

FPlayerData::FPlayerData(APawn* NewPlayerPawn) : FPlayerData()
{
	PlayerPawn = NewPlayerPawn;
	LastPosition = PlayerPawn->GetActorLocation();
}

bool FPlayerData::UpdatePositionData()
{
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed"));
		return false;
	}

	FVector CurrentLocation = PlayerPawn->GetActorLocation();

	// Remove the oldest distance travelled
	TotalMovement -= PlayerDistances[PlayerMovementIndex];

	// Add the newest distance travelled to array
	PlayerDistances[PlayerMovementIndex] = LastPosition - CurrentLocation;

	// Add the newest distance travelled
	TotalMovement += PlayerDistances[PlayerMovementIndex];

	// New position becomes the next last position
	LastPosition = CurrentLocation;

	// Move to next index in array
	PlayerMovementIndex = (PlayerMovementIndex + 1) % PLAYER_MOVEMENT_RECORD_SIZE;

	// Now check to see how far player has travelled
	Distance = (TotalMovement / PLAYER_MOVEMENT_RECORD_SIZE).Length();
	if (Distance < PLAYER_CAMPING_DISTANCE && PlayerCampingIndex > PLAYER_CAMPING_COOLDOWN)
	{
		UE_LOG(
			LogTemp, Log, 
			TEXT("Player is not on the move. Distance Travelled: %d"),
			Distance
		);

		PlayerCampingIndex = 0;
		return true;
	}

	return false;
}

FTokenCollection* FActorTokensCollection::GetCollectionOfType(ETokenType TokenType)
{
	switch (TokenType)
	{
		case ETokenType::Normal:
			return &NormalTokens;
		case ETokenType::Heavy:
			return &HeavyTokens;
		default:
			UE_LOG(LogTemp, Error, TEXT("FActorTokensCollection::GetCollectionOfType: Invalid Token Type provided."));
			return nullptr;
	}
}
