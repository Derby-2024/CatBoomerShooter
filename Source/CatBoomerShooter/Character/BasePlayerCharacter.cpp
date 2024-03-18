// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "../Environment/InteractInterface.h"
#include <GameFramework/MovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "Components/InputComponent.h"
#include "../AI/Director/AIDirectorGameMode.h"

#include "EMSAsyncSaveGame.h"
#include "EMSAsyncLoadGame.h"
#include "EMSFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "BaseCharacterMovementComponent.h"
#include "CatBoomerShooter/Weapons/Whip/BaseWhip.h"
#include "CatBoomerShooter/Weapons/BaseWeapon.h"

#include "CatBoomerShooter/AI/Director/AIDirectorGameMode.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	SK_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	SK_Arms->SetupAttachment(Camera);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AAIDirectorGameMode* GameMode = Cast<AAIDirectorGameMode>(GetWorld()->GetAuthGameMode()); 
	if (GameMode) { 
		GameMode->AddDefaultTokensToActor(this); 
	} 
	else { 
		UE_LOG(LogTemp, Error, TEXT("ABasePlayerCharacter::BeginPlay: Couldn't get ai game mode")) 
	}

	// When Spawning, We quickly load the player and level data, if level was not played before nothing will be affected
	QuickLoad();
}

void ABasePlayerCharacter::Destroyed()
{
	Super::Destroyed();
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* actor : AttachedActors)
	{
		actor->Destroy();
	}
}

void ABasePlayerCharacter::InputMove(const FInputActionValue& Value)
{
	const FVector2D MoveInputValue = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MoveInputValue.X);
	AddMovementInput(GetActorRightVector(), MoveInputValue.Y);

}

void ABasePlayerCharacter::TapDash(const FInputActionValue& Value){
	FVector2D DirectionValue = Value.Get<FVector2D>();
	if(DirectionValue.Equals(StoredDirectionValue) && !StoredDirectionValue.Equals(FVector2D::Zero())){
		Dash(DirectionValue);
		StoredDirectionValue = FVector2D::Zero();
	}
	else{
		StoredDirectionValue = DirectionValue;
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *StoredDirectionValue.ToString());
	GetWorldTimerManager().SetTimer(TapTimerHandle, this, &ABasePlayerCharacter::ResetStoredDirectionValue, TapIntervalTime,false, TapIntervalTime);
	
}
void ABasePlayerCharacter::InputJump(const FInputActionValue& Value)
{
	const bool ShouldJump = Value.Get<bool>();
	if (ShouldJump) {
		Jump();
	}
}

void ABasePlayerCharacter::Dash(const FInputActionValue& Value)
{
	//this happens if dash is pressed and the player has a dash available
	if(DashCount>0)
	{
		FVector2D InputVal = FVector2D::Zero();
		if(Value.GetValueType() ==EInputActionValueType::Boolean)
			InputVal = InputMoveVal->GetValue().Get<FVector2D>();
		else
			InputVal = Value.Get<FVector2D>();
			
		FVector DashVel = GetActorForwardVector()* InputVal.X + GetActorRightVector()* InputVal.Y;
		DashVel.Normalize();
		DashVel.Z = 0;
		DashVel = DashVel * DashSpeed;
		if((DashVel.X!=0) ||(DashVel.Y !=0)){
			if(!GetMovementComponent()->IsFalling()){
				//sets the player invincible
				IsInvincible = true;
				this -> LaunchCharacter(DashVel,false,false);
				//resets the players invincibility
				GetWorldTimerManager().SetTimer(InvTimerHandle, this, &ABasePlayerCharacter::ResetInvincibility, InvincibleDuration,false, InvincibleDuration);
				DashCount=DashCount-1;
				//ResetDashCounter();
				//this uses a timer to call the restdashcounter, the first DashCooldown is the time betwwen the first loop and the second loop the second dashCooldwon is time for the first loop to occur.
				GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ABasePlayerCharacter::ResetDashCounter,DashCooldown, true,DashCooldown);
			}
			
		}
	}
}
void ABasePlayerCharacter::ResetInvincibility()
{
	if(IsInvincible != false){
		IsInvincible = false;
	}
	
}

void ABasePlayerCharacter::ResetDashCounter()
{
	if (DashCount < 3){
		DashCount=DashCount+1;
	}
	
}

void ABasePlayerCharacter::ResetStoredDirectionValue()
{
	StoredDirectionValue = FVector2D::Zero();
	
}



void ABasePlayerCharacter::InputJumpEnd(const FInputActionValue& Value)
{

}

void ABasePlayerCharacter::InputCameraMove(const FInputActionValue& Value)
{
	const FVector2D CameraMoveInputValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(CameraMoveInputValue.X * MouseSensitivity);
		AddControllerPitchInput(CameraMoveInputValue.Y * MouseSensitivity);
	}
}

/*
	On Interact, check if hit actor implements the Interface and run function
*/
void ABasePlayerCharacter::InputInteract(const FInputActionValue& Value)
{
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + Camera->GetForwardVector() * InteractRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor())) {
		if (IInteract* HitInteract = Cast<IInteract>(Hit.GetActor())) {
			HitInteract->Execute_OnInteract(Hit.GetActor(), this);
		}
	}
}

void ABasePlayerCharacter::InputMelee(const FInputActionValue& Value)
{
	if(Whip)
	{
		Whip->Attack();
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No whip"));
	}
}

void ABasePlayerCharacter::InputFire_Start(const FInputActionValue &Value)
{
	if(Weapon)
	{
		Weapon->StartShooting();
	}
}

void ABasePlayerCharacter::InputFire_Stop(const FInputActionValue &Value)
{
	if(Weapon)
	{
		Weapon->StopShooting();
	}
}

void ABasePlayerCharacter::InputWeapon1(const FInputActionValue& Value)
{
	EquipWeapon(0);
}

void ABasePlayerCharacter::InputWeapon2(const FInputActionValue& Value)
{
	EquipWeapon(1);
}

void ABasePlayerCharacter::InputWeapon3(const FInputActionValue& Value)
{
	EquipWeapon(2);
}

void ABasePlayerCharacter::InputQuickSave(const FInputActionValue& Value)
{
	QuickSave();
}

void ABasePlayerCharacter::InputQuickLoad(const FInputActionValue& Value)
{
	QuickLoad();
}

void ABasePlayerCharacter::ComponentsToSave_Implementation(TArray<UActorComponent*>& Components)
{
	Components.Add(InventoryComponent);
	Components.Add(HealthComponent);
}

void ABasePlayerCharacter::ActorLoaded_Implementation()
{
	if (WeaponClassList.Num() > 0) {
		for (TSubclassOf<class ABaseWeapon> LoadedWeapon : WeaponClassList) {
			AddWeapon(LoadedWeapon);
		}
	}
	EquipWeapon(CurrentWeaponIndex);
}

void ABasePlayerCharacter::ActorPreSave_Implementation()
{
	WeaponClassList.Empty();

	for (ABaseWeapon* LoadedWeapon : WeaponList) {
		WeaponClassList.Add(LoadedWeapon->GetClass());
	}
}

void ABasePlayerCharacter::InputNextWeapon(const FInputActionValue& Value)
{
	if (WeaponList.Num() == 0)
	{
		return;
	}
	WeaponList[CurrentWeaponIndex]->StopShooting();
	CurrentWeaponIndex++;
	if (CurrentWeaponIndex >= WeaponList.Num())
	{
		CurrentWeaponIndex = 0;
	}
	EquipWeapon(CurrentWeaponIndex);
}

void ABasePlayerCharacter::InputPreviousWeapon(const FInputActionValue& Value)
{
	if (WeaponList.Num() == 0)
	{
		return;
	}
	WeaponList[CurrentWeaponIndex]->StopShooting();
	CurrentWeaponIndex--;
	if (CurrentWeaponIndex < 0)
	{
		CurrentWeaponIndex = WeaponList.Num() - 1;
	}
	EquipWeapon(CurrentWeaponIndex);
}

void ABasePlayerCharacter::InputPause(const FInputActionValue& Value)
{

}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Using enhanced input system
	// Bind Input to functions
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::TapDash);
		if(EnableAutoJump)
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputJump);
		else
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::InputJumpEnd);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputCameraMove);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputInteract);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputMelee);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputFire_Start);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::InputFire_Stop);
		EnhancedInputComponent->BindAction(Weapon1, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputWeapon1);
		EnhancedInputComponent->BindAction(Weapon2, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputWeapon2);
		EnhancedInputComponent->BindAction(Weapon3, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputWeapon3);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputPause);
		EnhancedInputComponent->BindAction(NextWeapon, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputNextWeapon);
		EnhancedInputComponent->BindAction(PreviousWeapon, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputPreviousWeapon);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Dash);

		EnhancedInputComponent->BindAction(QuickSaveAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputQuickSave);
		EnhancedInputComponent->BindAction(QuickLoadAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputQuickLoad);

		InputMoveVal = &EnhancedInputComponent->BindActionValue(MoveAction);
		InputCameraMoveVal = &EnhancedInputComponent->BindActionValue(CameraMoveAction);
	}
}

USkeletalMeshComponent *ABasePlayerCharacter::GetPlayerArms_Implementation()
{
    return SK_Arms;
}

ABaseWhip *ABasePlayerCharacter::GetPlayerWhip_Implementation()
{
    return Whip;
}

UCameraComponent *ABasePlayerCharacter::GetPlayerCamera_Implementation()
{
	return Camera;
}

void ABasePlayerCharacter::EquipWeapon(int WeaponIndex)
{
	if (!WeaponList.IsValidIndex(WeaponIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a valid weapon index"));
		return;
	}

	WeaponList[CurrentWeaponIndex]->StopShooting();

	CurrentWeaponIndex = WeaponIndex;
	for (ABaseWeapon* weapon : WeaponList)
	{
		if (WeaponList[CurrentWeaponIndex] == weapon)
		{
			weapon->SetActorHiddenInGame(false);
			Weapon = weapon;
		}
		else
		{
			weapon->SetActorHiddenInGame(true);
		}
	}
}

int ABasePlayerCharacter::AddWeapon(TSubclassOf<class ABaseWeapon> WeaponClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	return WeaponList.AddUnique(GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, GetActorTransform(), SpawnParams));
}

void ABasePlayerCharacter::QuickSave() 
{
	int32 Flags = ENUM_TO_FLAG(ESaveTypeFlags::SF_Level) | ENUM_TO_FLAG(ESaveTypeFlags::SF_Player);

	auto EMSAsyncSaveGame = UEMSAsyncSaveGame::AsyncSaveActors(this, Flags);
	if (IsValid(EMSAsyncSaveGame)) {
		EMSAsyncSaveGame->Activate();
	}
}

void ABasePlayerCharacter::QuickLoad()
{
	/*TSubclassOf<ABaseWeaponProjectile> Projectileclass = ABaseWeaponProjectile::StaticClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Projectileclass, FoundActors);

	TSubclassOf<ABaseWeapon> WeaponClass = ABaseWeapon::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WeaponClass, FoundActors);

	for (AActor* a : FoundActors) {
		a->Destroy();
	}

	WeaponList.Empty();*/

	int32 Flags = ENUM_TO_FLAG(ESaveTypeFlags::SF_Level) | ENUM_TO_FLAG(ESaveTypeFlags::SF_Player);

	auto EMSAsyncLoadGame = UEMSAsyncLoadGame::AsyncLoadActors(this, Flags, true);
	if (IsValid(EMSAsyncLoadGame)) {
		EMSAsyncLoadGame->Activate();
	}

	// A better aproach that reloads the whole level but requires GameMode implementation to load everything properly
	//FString SaveGameName;

	//UEMSInfoSaveGame* SaveSlot = UEMSFunctionLibrary::GetSlotInfoSaveGame(this, SaveGameName);

	//UGameplayStatics::OpenLevel(this, SaveSlot->SlotInfo.Level);
}
