// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BasePlayerInterface.h"

#include "BasePlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CATBOOMERSHOOTER_API ABasePlayerCharacter : public ACharacter, public IBasePlayerInterface
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* WhipLocation;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MeleeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DashAction;
	UInputAction* FireAction;
	
	// Input Functions
	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputJumpEnd(const FInputActionValue& Value);
	void InputCameraMove(const FInputActionValue& Value);
	void InputMelee(const FInputActionValue& Value);
	void InputFire_Start(const FInputActionValue& Value);
	void InputFire_Stop(const FInputActionValue& Value);

	//Whip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Whip")
 	class ABaseWhip* Whip;
	void Dash(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
 	class ABaseWeapon* Weapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	USceneComponent* GetPlayerWhipLocation(); virtual USceneComponent* GetPlayerWhipLocation_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	ABaseWhip* GetPlayerWhip(); virtual ABaseWhip* GetPlayerWhip_Implementation() override;


};
