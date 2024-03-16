// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Weapon1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Weapon2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Weapon3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* NextWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PreviousWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction;

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
	
	// Input Functions
	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputJumpEnd(const FInputActionValue& Value);
	void InputCameraMove(const FInputActionValue& Value);
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

	//Whip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Whip")
 	class ABaseWhip* Whip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
 	class ABaseWeapon* Weapon;
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<ABaseWeapon*> WeaponList = {};
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<class ABaseWeapon>> WeaponClassList = {};

	//Triggers out of ammo notification that they are out of ammo
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void TriggerOutOfAmmo();

	// Reference to the InventoryComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent;


public:
	UPROPERTY(EditAnywhere)
	float InteractRange = 500.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	USkeletalMeshComponent* GetPlayerArms(); virtual USkeletalMeshComponent* GetPlayerArms_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Whip Interface")
	ABaseWhip* GetPlayerWhip(); virtual ABaseWhip* GetPlayerWhip_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera")
	UCameraComponent* GetPlayerCamera(); virtual UCameraComponent* GetPlayerCamera_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(int WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int AddWeapon(TSubclassOf<class ABaseWeapon> WeaponClass);

	void ComponentsToSave_Implementation(TArray<UActorComponent*>& Components) override;

	// Expose save and load
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void QuickSave();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void QuickLoad();
	
	void ActorLoaded_Implementation() override;
	void ActorPreSave_Implementation() override;
};
