// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

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

void ABasePlayerCharacter::TapDash(const FInputActionValue& Value){
	FVector2D DirectionValue = Value.Get<FVector2D>();
	if (NumOfTaps == 0){
		StoredDirectionValue = DirectionValue;
	}
	NumOfTaps = NumOfTaps + 1;
	// makes sure the player doesnt spam a directional key and dash multiple times
	if(NumOfTaps==2){
		if(DirectionValue == StoredDirectionValue){
			if(DirectionValue.X!=0){
				DirectionValue.X=500;
				Dash(StoredDirectionValue);
				NumOfTaps=0;
			}
			if(DirectionValue.Y!=0){
				DirectionValue.Y=500;
				Dash(StoredDirectionValue);
				NumOfTaps=0;
			}
		}
		NumOfTaps=0;
	}
	GetWorldTimerManager().SetTimer(TapTimerHandle, this, &ABasePlayerCharacter::ResetNumOfTaps, 2.0f,true, 2.0f);
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
		FVector DashVel = GetVelocity();
		DashVel.Normalize();
		DashVel.Z = 0;
		DashVel = DashVel * DashSpeed;
		if((DashVel.X!=0) ||(DashVel.Y !=0)){
			bool IsFalling = GetMovementComponent()->IsFalling();
			if(IsFalling == false){
				//sets the player invincible
				IsInvincible = true;
				this -> LaunchCharacter(DashVel,false,false);
				//resets the players invincibility
				GetWorldTimerManager().SetTimer(InvTimerHandle, this, &ABasePlayerCharacter::ResetInvincibility, 1.0f,true, InvincibleDuration);
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

void ABasePlayerCharacter::ResetNumOfTaps()
{
	NumOfTaps = 0 ;
	
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::InputJump);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::InputCameraMove);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Dash);
	}
}

