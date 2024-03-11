// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "BasePlayerInterface.h"
#include "EMSActorSaveInterface.h"
#include "../Inventory/InventoryComponent.h"
#include "BasePlayerCharacter.generated.h"



class UInputMappingContext;
class UInputAction;

UCLASS()
class CATBOOMERSHOOTER_API ABasePlayerCharacter : public ACharacter, public IBasePlayerInterface, public IEMSActorSaveInterface
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
	class USkeletalMeshComponent* SK_Arms;

	// Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CameraMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MeleeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;

	// EMS Quick Load/Save Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuickSaveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuickLoadAction;

	// Input Variables
	UPROPERTY(EditAnywhere, Category = Input)
	bool EnableAutoJump = true;

	UPROPERTY(EditAnywhere, Category = Input)
	float MouseSensitivity = 1.0f;

	struct FEnhancedInputActionValueBinding* InputMoveVal;
	struct FEnhancedInputActionValueBinding* InputCameraMoveVal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DashAction;

	//variables for dashing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	int DashCount=3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	int DashSpeed=25000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float DashCooldown = 2.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool IsInvincible = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float InvincibleDuration = 0.15;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	int NumOfTaps = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FVector2D StoredDirectionValue;

	FTimerHandle DashTimerHandle;
	FTimerHandle InvTimerHandle;
	FTimerHandle TapTimerHandle;

	// Input Functions
	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputJumpEnd(const FInputActionValue& Value);
	void InputCameraMove(const FInputActionValue& Value);
	void InputInteract(const FInputActionValue& Value);
	void InputMelee(const FInputActionValue& Value);
	void InputFire_Start(const FInputActionValue& Value);
	void InputFire_Stop(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void TapDash(const FInputActionValue& Value);
	void InputQuickSave(const FInputActionValue& Value);
	void InputQuickLoad(const FInputActionValue& Value);

	//Whip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Whip")
 	class ABaseWhip* Whip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
 	class ABaseWeapon* Weapon;

	//Reload Weapon
	void ReloadWeapon();

	//Triggers out of ammo notification that they are out of ammo
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void TriggerOutOfAmmo();

	// Reference to the InventoryComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

public:
	UPROPERTY(EditAnywhere)
	float InteractRange = 500.0f;

	//test health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	//Temporary variable until inventory system is finished
	UPROPERTY(EditAnywhere, SaveGame)
	TArray<FString> Keys;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void TrainImpact(FVector PushDirection, float PushForce);

	void ResetDashCounter();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	USkeletalMeshComponent* GetPlayerArms(); virtual USkeletalMeshComponent* GetPlayerArms_Implementation() override;

	void ResetInvincibility();

	void ResetNumOfTaps();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	ABaseWhip* GetPlayerWhip(); virtual ABaseWhip* GetPlayerWhip_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera")
	UCameraComponent* GetPlayerCamera(); virtual UCameraComponent* GetPlayerCamera_Implementation() override;

	void ComponentsToSave_Implementation(TArray<UActorComponent*>& Components) override;
};
