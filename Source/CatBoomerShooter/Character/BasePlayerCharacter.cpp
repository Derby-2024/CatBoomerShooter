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

	SK_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	SK_Arms->SetupAttachment(Camera);
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
