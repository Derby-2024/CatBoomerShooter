// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BasePlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CATBOOMERSHOOTER_API ABasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	// ABasePlayerCharacter();
	ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* WishDirArrow;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* AccelDirArrow;

	// Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CameraMoveAction;

	// Input Variables
	UPROPERTY(EditAnywhere, Category = Input)
	bool EnableAutoJump = true;

	UPROPERTY(EditAnywhere, Category = Input)
	float MouseSensitivity = 1.0f;

	struct FEnhancedInputActionValueBinding* InputMoveVal;
	struct FEnhancedInputActionValueBinding* InputCameraMoveVal;
	
	// Input Functions
	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputJumpEnd(const FInputActionValue& Value);
	void InputCameraMove(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
