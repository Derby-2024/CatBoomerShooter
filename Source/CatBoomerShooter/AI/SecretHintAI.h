// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CatPath.h"
#include "SecretHintAI.generated.h"


UCLASS()
class CATBOOMERSHOOTER_API ASecretHintAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASecretHintAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class ACatPath* CCatPath;
private: 
	float hintMoveSpeed = 5.0f;
	float activeMoveDist = 10.0f;
	int y = 0;
	int x = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
