// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "../Environment/InteractInterface.h"
#include "Components/InputComponent.h"
#include "BaseCharacterMovementComponent.h"
#include <GameFramework/MovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "CatBoomerShooter/Whip/BaseWhip.h"
#include "CatBoomerShooter/Weapons/BaseWeapon.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayerCharacter::InputMove(const FInputActionValue& Value)
{
	const FVector2D MoveInputValue = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MoveInputValue.X);
	AddMovementInput(GetActorRightVector(), MoveInputValue.Y);

}

void ABasePlayerCharacter::InputJump(const FInputActionValue& Value)
{
	const bool ShouldJump = Value.Get<bool>();
	if (ShouldJump) {
		Jump();
	}
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
		if(EnableAutoJump)
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputJump);
		else
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayerCharacter::InputJumpEnd);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputCameraMove);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputInteract);

		InputMoveVal = &EnhancedInputComponent->BindActionValue(MoveAction);
		InputCameraMoveVal = &EnhancedInputComponent->BindActionValue(CameraMoveAction);
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