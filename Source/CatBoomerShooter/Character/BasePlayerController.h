// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API ABasePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts of when spawned
	virtual void BeginPlay() override;

protected:
	// Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

};
