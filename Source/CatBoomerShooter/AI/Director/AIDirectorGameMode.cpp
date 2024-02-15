// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDirectorGameMode.h"

//FEnemyToken AAIDirectorGameMode::RequestToken(ETokenType TokenType, AAIEnemyBaseController* EnemyController, AActor* TargetActor)
void AAIDirectorGameMode::RequestToken(const AAIEnemyBaseController* EnemyController, const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, FEnemyToken& Token, bool& Success)
{
	Success = true;
	Token = BasicToken;
}

bool AAIDirectorGameMode::ReleaseToken()
{
	return false;
}