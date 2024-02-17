// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include <GameFramework/MovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	WishDirArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("WishDirArrow"));
	WishDirArrow->SetupAttachment(RootComponent);

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

	AddMovementInput(GetActorForwardVector(), MoveInputValue.X);
	AddMovementInput(GetActorRightVector(), MoveInputValue.Y);

}

void ABasePlayerCharacter::InputJump(const FInputActionValue& Value)
{
	const bool ShouldJump = Value.Get<bool>();
	if (ShouldJump) {
		Jump();
		IsOnGround = false;
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

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D moveInput = InputMoveVal->GetValue().Get<FVector2D>();
	FVector2D mouseInput = InputCameraMoveVal->GetValue().Get<FVector2D>();

	FVector MoveDir = GetActorForwardVector() * moveInput.X + GetActorRightVector() * moveInput.Y;
	FVector MouseDir = Camera->GetForwardVector();
	MouseDir.Z = 0;

	MoveDir += MouseDir;
	MoveDir.Normalize();
		
	if (!IsOnGround) {
		UMovementComponent* MovementComponent = Cast<UMovementComponent>(this->GetComponentByClass(UMovementComponent::StaticClass()));
		MovementComponent->Velocity = Accelerate(MoveDir, GetVelocity());
	}
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
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputCameraMove);

		InputMoveVal = &EnhancedInputComponent->BindActionValue(MoveAction);
		InputCameraMoveVal = &EnhancedInputComponent->BindActionValue(CameraMoveAction);
	}
}

void ABasePlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	IsOnGround = true;
}

// Strafe project acceleration velocity
FVector ABasePlayerCharacter::Accelerate(FVector WishedDirection, FVector PrevVelocity) const
{
	float projVelocity = PrevVelocity.Dot(WishedDirection);
	float AccelVel = MaxAcceleration * GetWorld()->GetDeltaSeconds();

	if (projVelocity + AccelVel > MaxVel) {
		AccelVel = MaxVel - projVelocity;
	}

	return PrevVelocity + WishedDirection * AccelVel;
}

