// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "BasePlayerInterface.h"
#include "EMSActorSaveInterface.h"
#include "../Inventory/InventoryComponent.h"
#include "../Utilities/HealthComponent.h"
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

	virtual void Destroyed() override;

	// Input Functions
	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputJumpEnd(const FInputActionValue& Value);
	void InputCameraMove(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void TapDash(const FInputActionValue& Value);
	void InputInteract(const FInputActionValue& Value);
	void InputMelee(const FInputActionValue& Value);
	void InputFire_Start(const FInputActionValue& Value);
	void InputFire_Stop(const FInputActionValue& Value);
	void InputWeapon1(const FInputActionValue& Value);
	void InputWeapon2(const FInputActionValue& Value);
	void InputWeapon3(const FInputActionValue& Value);
	void InputQuickSave(const FInputActionValue& Value);
	void InputQuickLoad(const FInputActionValue& Value);
	void InputNextWeapon(const FInputActionValue& Value);
	void InputPreviousWeapon(const FInputActionValue& Value);
	void InputPause(const FInputActionValue& Value);

protected:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* ArmsMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent;

	// Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CameraMoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DashAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MeleeAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Weapon1Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Weapon2Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Weapon3Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* NextWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PreviousWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuickSaveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuickLoadAction = nullptr;

	struct FEnhancedInputActionValueBinding* InputMoveVal;
	struct FEnhancedInputActionValueBinding* InputCameraMoveVal;

public:
	// Input Variables
	UPROPERTY(EditAnywhere, Category = Input)
	float InteractRange = 500.0f;
	UPROPERTY(EditAnywhere, Category = Input)
	bool bEnableAutoJump = true;
	UPROPERTY(EditAnywhere, Category = Input)
	float MouseSensitivity = 1.0f;

	//variables for dashing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	int DashCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	int DashSpeed = 25000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldown = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float TapIntervalTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	bool IsInvincible = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float InvincibleDuration = 0.15f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	FVector2D StoredDirectionValue;

	FTimerHandle DashTimerHandle;
	FTimerHandle InvTimerHandle;
	FTimerHandle TapTimerHandle;

	// Weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ABaseWhip* Whip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ABaseWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<ABaseWeapon*> WeaponList = {};
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<class ABaseWeapon>> WeaponClassList = {};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Dash Functions
	void ResetDashCounter();
	void ResetInvincibility();
	void ResetStoredDirectionValue();

	// Quick Save and Load Functions
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void QuickSave();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void QuickLoad();

	// Weapon Functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(int WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int AddWeapon(TSubclassOf<class ABaseWeapon> WeaponClass);

	void SetWhip(ABaseWhip* NewWhip) { Whip = NewWhip; }

	//Triggers out of ammo notification that they are out of ammo
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void TriggerOutOfAmmo();

	// Interface Implementaions

	// Weapon Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	USkeletalMeshComponent* GetPlayerArms(); virtual USkeletalMeshComponent* GetPlayerArms_Implementation() override { return GetMesh(); };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	ABaseWhip* GetPlayerWhip(); virtual ABaseWhip* GetPlayerWhip_Implementation() override { return Whip; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera")
	UCameraComponent* GetPlayerCamera(); virtual UCameraComponent* GetPlayerCamera_Implementation() override { return Camera; };

	// EMS Actor Interface
	void ComponentsToSave_Implementation(TArray<UActorComponent*>& Components) override;
	void ActorLoaded_Implementation() override;
	void ActorPreSave_Implementation() override;
};
