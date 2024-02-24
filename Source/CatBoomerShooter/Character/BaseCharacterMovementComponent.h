// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	//UBaseCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	void InitializeComponent() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool DoJump(bool bReplayingMoves) override;

	void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
	float GetMaxSpeed() const override;
	float GetMaxAcceleration() const override;

	void ApplyTrimpingVelocity(const FFindFloorResult& Floor);
	void ApplyAcceleration(float DeltaTime, float SurfaceFriction, const FVector& WishDirection, float WishSpeed, float accel);
	void ApplyAirAcceleration(float DeltaTime, float SurfaceFriction, const FVector& WishDirection, float WishSpeed, float MaxAirWishSpeed, float accel);
	void ApplyFriction(float DeltaTime, float CharacterFriction, float SurfaceFriction, float SpeedStop);
	bool ShouldApplyGroundFriction();

	// How much time (in seconds) that ground friction should be ignored after landing.
	UPROPERTY(Category = "Character Movement: Bunnyhopping", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float NoFrictionAfterLandingTime = 0.0f;

	// Timestamp since character was last in the air.
	float LastAirTimestamp;

	// Whether or not to enforce an absolute maximum speed limit that the player cannot bypass through bunnyhopping or strafe-jumping.
	UPROPERTY(Category = "Character Movement: Bunnyhopping", EditAnywhere, BlueprintReadWrite)
	bool bUseEnforcedMaxSpeed = true;

	// If bUseEnforcedMaxSpeed is true, the value of this variable will be used as the enforced maximum speed limit for the player.
	UPROPERTY(Category = "Character Movement: Bunnyhopping", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseEnforcedMaxSpeed", ClampMin = "0", UIMin = "0"))
	float EnforcedMaxSpeed = 3500.0f;

	// The maximum speed that a player can have while jumping or falling while in the air. Can be bypassed by bunnyhopping or strafe-jumping.
	UPROPERTY(Category = "Character Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxFallAirSpeed = 100.0f;

	// The max acceleration gained when jumping / falling in the air.
	UPROPERTY(Category = "Character Movement: Jumping / Falling", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxAirAcceleration = 20.0f;

	// If the character's speed is lower than this speed, then friction that is scaled with this speed will be applied instead. This allows for snappier stopping of movement.
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float StopSpeed = 150.0f;

	// Whether or not trimping should be enabled.
	// Trimping is when you get height gains on ramps facing upwards or speed gains on ramps facing downwards from the player.
	// The amount of speed / height you gain depends on the slope of the incline and the current speed.
	UPROPERTY(Category = "Character Movement: Trimping", EditAnywhere, BlueprintReadWrite)
	bool bEnableTrimping = true;

	// If trimping on a slope inclined downwards, this value is used in order to determine the lowest possible jump velocity. The higher this value, the less the min jump velocity.
	UPROPERTY(Category = "Character Movement: Trimping", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableTrimping", ClampMin = "0", UIMin = "0"))
	float MaxTrimpJumpHeightReductionMultiplier = 0.375f;

	// If trimping on a slope inclined upwards, cap the max vertical velocity you can gain (in the Z direction).
	UPROPERTY(Category = "Character Movement: Trimping", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableTrimping", ClampMin = "0", UIMin = "0"))
	float MaxTrimpVerticalVelocityBoost = 2000.0f;

	// Multiplies the final velocity boost gained by trimping on a slope inclined upwards by this value.
	UPROPERTY(Category = "Character Movement: Trimping", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableTrimping", ClampMin = "0", UIMin = "0"))
	float TrimpVerticalVelocityBoostMultiplier = 1.25f;

	// If trimping on a slope inclined downwards, cap the max horiz speed you can gain (in the X and Y direction).
	UPROPERTY(Category = "Character Movement: Trimping", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableTrimping", ClampMin = "0", UIMin = "0"))
	float MaxTrimpHorizSpeedBoost = 1000.0f;

	// Multiplies the final speed boost gained by trimping on a slope inclined downwards by this value.
	UPROPERTY(Category = "Character Movement: Trimping", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableTrimping", ClampMin = "0", UIMin = "0"))
	float TrimpHorizSpeedBoostMultiplier = 2.75f;
};
