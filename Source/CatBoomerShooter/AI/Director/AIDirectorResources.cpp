// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDirectorResources.h"
#include "../EnemyBase.h"

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
