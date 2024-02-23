// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameTeams.h"
#include "AIEnemyBaseController.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AAIEnemyBaseController : public AAIController
{
	GENERATED_BODY()

public:
	AAIEnemyBaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyBase")
	class UBehaviorTree* DefaultBehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Teams")
	EGameTeam GameTeam = EGameTeam::Aliens;

private:
	/** Requests an enemy token of a given type.
	*	Token Priority is currently unimplemented. */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void RequestToken(const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, UEnemyToken*& Token, bool& Success);

	/** Returns a token from being used by an enemy */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void ReleaseToken(UEnemyToken* Token, const float CustomCooldown = -1.0f);

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Tokens")
	void TokenRetracted(UEnemyToken* Token);

	// AIController Team functions
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
};
