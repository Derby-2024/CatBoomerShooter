// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "CatBoomerShooter/Whip/BaseWhip.h"
#include "CatBoomerShooter/Weapons/BaseWeapon.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	WhipLocation = CreateDefaultSubobject<USceneComponent>(TEXT("WhipLocation"));
	WhipLocation->SetupAttachment(Camera);

	//Set default value for variable
	Weapon = nullptr;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	
}

void ABasePlayerCharacter::InputMove(const FInputActionValue& Value)
{
	const FVector2D MoveInputValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddMovementInput(GetActorForwardVector(), MoveInputValue.X);
		AddMovementInput(GetActorRightVector(), MoveInputValue.Y);
	}
}

void ABasePlayerCharacter::InputJump(const FInputActionValue& Value)
{
	const bool ShouldJump = Value.Get<bool>();
	if (ShouldJump) {
		Jump();
	}
}

void ABasePlayerCharacter::InputCameraMove(const FInputActionValue& Value)
{
	const FVector2D CameraMoveInputValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(CameraMoveInputValue.X);
		AddControllerPitchInput(CameraMoveInputValue.Y);
	}
}

void ABasePlayerCharacter::InputMelee(const FInputActionValue& Value)
{
	if(Whip)
	{
		Whip->Attack();
	}
}

void ABasePlayerCharacter::InputFire_Start(const FInputActionValue &Value)
{
	if(Weapon)
	{
		if (Weapon->CurrentAmmo > 0)
		{
			Weapon->StartShooting();

			//Decrease Clip Ammo by 1
			Weapon->CurrentAmmo -= 1;
		}
		else if (Weapon->CurrentAmmo < 0)
		{
			ReloadWeapon();
		}
		else
		{
			TriggerOutOfAmmo();
		}
	}
}

void ABasePlayerCharacter::InputFire_Stop(const FInputActionValue &Value)
{
	if(Weapon)
	{
		Weapon->StopShooting();
	}
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputJump);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputCameraMove);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputMelee);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputFire_Start);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::InputFire_Stop);
	}
	
	//Bind Reload Event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasePlayerCharacter::ReloadWeapon);
}

USceneComponent *ABasePlayerCharacter::GetPlayerWhipLocation_Implementation()
{
    return WhipLocation;
}

ABaseWhip *ABasePlayerCharacter::GetPlayerWhip_Implementation()
{
    return Whip;
}


void ABasePlayerCharacter::ReloadWeapon()
{
	if (Weapon)
	{
		if (Weapon->CurrentAmmo < Weapon->TotalAmmo && Weapon->AmmoAmount > 0)
		{
			Weapon->CurrentAmmo += Weapon->AmmoAmount;
			Weapon->AmmoAmount -= Weapon->AmmoAmount;
		}
		else
		{
			Weapon->StopShooting();
		}
	}
}

//if (Weapon->ClipAmmo != Weapon->MaxClipAmmo) //Checking if we have room to reload 
//{
//	if (Weapon->TotalAmmo - (Weapon->MaxClipAmmo - Weapon->ClipAmmo) >= 0)
//	{
//		Weapon->TotalAmmo -= (Weapon->MaxClipAmmo - Weapon->ClipAmmo); //Check how much to reload
//		Weapon->ClipAmmo = Weapon->MaxClipAmmo; 
//	}
//	else
//	{
//		//Use the remaining ammo 
//		Weapon->ClipAmmo += Weapon->TotalAmmo;			
//		Weapon->TotalAmmo = 0;
//	}
//}