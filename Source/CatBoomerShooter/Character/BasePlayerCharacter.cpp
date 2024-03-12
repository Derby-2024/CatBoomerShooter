// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "../Environment/InteractInterface.h"
#include <GameFramework/MovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "Components/InputComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "CatBoomerShooter/Weapons/Whip/BaseWhip.h"
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

	SK_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	SK_Arms->SetupAttachment(Camera);

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

	// if (NumOfTaps == 0){
	// 	StoredDirectionValue = DirectionValue;
	// 	UE_LOG(LogTemp, Log, TEXT("%s"), *StoredDirectionValue.ToString());
	// 	NumOfTaps++;
	// }
	
	// // makes sure the player doesnt spam a directional key and dash multiple times
	// if(NumOfTaps==1){
	// 	if(DirectionValue.Equals(StoredDirectionValue)){
	// 		Dash(DirectionValue);
	// 		NumOfTaps=0;
	// 	}
	// 	else{
	// 		NumOfTaps = 0;
	// 		StoredDirectionValue = DirectionValue;
	// 	}
	//	GetWorldTimerManager().SetTimer(TapTimerHandle, this, &ABasePlayerCharacter::ResetNumOfTaps, 2.0f,true, 2.0f);
	// 	
	// }
	
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
		FVector DashVel = FVector(Value.Get<FVector2D>(), 0);
		DashVel.Normalize();
		DashVel.Z = 0;
		DashVel = DashVel * DashSpeed;
		if((DashVel.X!=0) ||(DashVel.Y !=0)){
			if(!GetMovementComponent()->IsFalling()){
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
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABasePlayerCharacter::Dash);

		InputMoveVal = &EnhancedInputComponent->BindActionValue(MoveAction);
		InputCameraMoveVal = &EnhancedInputComponent->BindActionValue(CameraMoveAction);
	}
	
	//Bind Reload Event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasePlayerCharacter::ReloadWeapon);
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


void ABasePlayerCharacter::ReloadWeapon()
{

}