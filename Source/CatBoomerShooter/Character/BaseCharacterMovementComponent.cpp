// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"

// Version that does not use inverse sqrt estimate, for higher precision.
FORCEINLINE FVector GetSafeNormalPrecise(const FVector& V)
{
	const auto VSq = V.SizeSquared();
	if (VSq < SMALL_NUMBER)
	{
		return FVector::ZeroVector;
	}
	else
	{
		return V * (1.f / FMath::Sqrt(VSq));
	}
}

// Version that does not use inverse sqrt estimate, for higher precision.
FORCEINLINE FVector GetClampedToMaxSizePrecise(const FVector& V, float MaxSize)
{
	if (MaxSize < KINDA_SMALL_NUMBER)
	{
		return FVector::ZeroVector;
	}

	const auto VSq = V.SizeSquared();
	if (VSq > FMath::Square(MaxSize))
	{
		return V * (MaxSize / FMath::Sqrt(VSq));
	}
	else
	{
		return V;
	}
}

void UBaseCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Allow full control of the character when looking while in the air.
	AirControl = 1.0f;
	AirControlBoostMultiplier = 1.0f;
	AirControlBoostVelocityThreshold = 0.0f;

	// This character can crouch.
	GetNavAgentPropertiesRef().bCanCrouch = true;
	bCanWalkOffLedgesWhenCrouching = true;

	// Can walk on slopes inclined at a max angle of 45 deg
	SetWalkableFloorAngle(45.0f);

	// Use a flat base for floor checks so that the character cannot "hang" off ledges because of the
	// roundish base that the capsule uses.
	bUseFlatBaseForFloorChecks = true;

	// Ground accel.
	MaxAcceleration = 6.0f;

	MaxWalkSpeed = 700.0f;
	GroundFriction = 5.0f;
	JumpZVelocity = 380.0f;
}

void UBaseCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsFalling())
		LastAirTimestamp = GetWorld()->GetTimeSeconds();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UBaseCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	const auto floor = CurrentFloor;
	const auto bJumped = Super::DoJump(bReplayingMoves);

	if (bJumped) {
		ApplyTrimpingVelocity(floor);
	}

	return bJumped;
}

void UBaseCharacterMovementComponent::ApplyTrimpingVelocity(const FFindFloorResult& Floor)
{
	// Apply trimping if we were on an incline.
	if (bEnableTrimping && Floor.bBlockingHit)
	{
		const auto HorizVelocity = FVector(Velocity.X, Velocity.Y, 0.0f);
		const auto HorizSpeed = HorizVelocity.Size();

		// Check that we actually have some speed in horiz direction.
		if (HorizSpeed > SMALL_NUMBER)
		{
			const auto FloorNormal = Floor.HitResult.ImpactNormal;
			const auto HorizVeloDirection = GetSafeNormalPrecise(HorizVelocity);

			// Get the angle between the normal of the slope and the horizontal direction of our velocity.
			// If slope is inclined upwards from our horiz velo direction, angle will be > pi/2
			// If perpendicular, angle = pi/2 aka 90 deg (dot product = 0)
			// If slope is inclined downwards from our horiz velo direction, angle will be < pi/2
			// Range of acos is between 0 & pi. 
			const auto Angle = FMath::Acos(FVector::DotProduct(FloorNormal, HorizVeloDirection));

			// If slope is inclined upwards from us. Give a height boost!
			// If the slope is inclined downwards, we'll lose some speed instead.
			const auto VerticalBoostSlopeMultiplier = ((2.0f * Angle) / PI) - 1.0f;
			auto VerticalVelocityBoost = FMath::Min(HorizSpeed * VerticalBoostSlopeMultiplier * TrimpVerticalVelocityBoostMultiplier, MaxTrimpVerticalVelocityBoost);
			VerticalVelocityBoost = FMath::Max(JumpZVelocity * -MaxTrimpJumpHeightReductionMultiplier, VerticalVelocityBoost);

			Velocity.Z += VerticalVelocityBoost;

			// If on a slope inclined below us, apply a horizontal speed boost too.
			if (Angle < (PI * 0.5f))
			{
				// Slope is inclined below us. Start to give a horizontal speed boost.
				const auto HorizBoostSlopeMultiplier = 1.0f - ((2.0f * Angle) / PI);
				const auto HorizSpeedBoost = FMath::Min(HorizSpeed * HorizBoostSlopeMultiplier * TrimpHorizSpeedBoostMultiplier, MaxTrimpHorizSpeedBoost);
				const auto HorizVelocityBoost = HorizVeloDirection * HorizSpeedBoost;

				Velocity.X += HorizVelocityBoost.X;
				Velocity.Y += HorizVelocityBoost.Y;
			}
		}
	}
}

void UBaseCharacterMovementComponent::ApplyAcceleration(float DeltaTime, float SurfaceFriction, const FVector& WishDirection, float WishSpeed, float accel)
{
	const auto VeloProj = FVector::DotProduct(Velocity, WishDirection);
	const auto AddSpeed = WishSpeed - VeloProj;
	if (AddSpeed <= 0.0f)
	{
		return;
	}

	auto AccelSpeed = accel * WishSpeed * SurfaceFriction * DeltaTime;
	AccelSpeed = FMath::Min(AccelSpeed, AddSpeed);

	Velocity += AccelSpeed * WishDirection;
}

void UBaseCharacterMovementComponent::ApplyAirAcceleration(float DeltaTime, float SurfaceFriction, const FVector& WishDirection, float WishSpeed, float MaxAirWishSpeed, float accel)
{
	const auto AirWishSpeed = FMath::Min(WishSpeed, MaxAirWishSpeed);

	const auto VeloProj = FVector::DotProduct(Velocity, WishDirection);
	const auto AddSpeed = AirWishSpeed - VeloProj;
	if (AddSpeed <= 0.0f)
	{
		return;
	}

	auto AccelSpeed = accel * WishSpeed * SurfaceFriction * DeltaTime;
	AccelSpeed = FMath::Min(AccelSpeed, AddSpeed);

	Velocity += AccelSpeed * WishDirection;
}

void UBaseCharacterMovementComponent::ApplyFriction(float DeltaTime, float CharacterFriction, float SurfaceFriction, float SpeedStop)
{
	const auto Speed = Velocity.Size();
	// Check if the speed is too small to care about...
	if (Speed < 10.0f)
	{
		Velocity = FVector::ZeroVector;
		return;
	}

	auto ControlSpeed = FMath::Max(SpeedStop, Speed);
	auto SpeedDrop = ControlSpeed * CharacterFriction * SurfaceFriction * DeltaTime;

	Velocity *= FMath::Max(0.0f, Speed - SpeedDrop) / Speed;
}

bool UBaseCharacterMovementComponent::ShouldApplyGroundFriction()
{
	return (IsMovingOnGround() && // If moving on the ground
		GetWorld()->TimeSeconds >= LastAirTimestamp + NoFrictionAfterLandingTime // If timeframe for ignoring friction after landing has expired.
		);
}

void UBaseCharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	// Do not update velocity when using root motion
	if (!HasValidData() || DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	// NOTE Restore the last movement mode so that friction and accel uses falling / jumping values
	// for at least one extra frame so that bunnyhopping works without "jerking" as soon as the player
	// hits the ground and quickly jumps again.
	static auto LastCalcVeloMovementMode = MOVE_None;
	const auto CurrentMovementMode = MovementMode;
	if (LastCalcVeloMovementMode == MOVE_Falling)
	{
		// NOTE Not using SetMovementMode() so that other code doesn't get notified.
		MovementMode = LastCalcVeloMovementMode;
	}

	// Ensure that friction isn't negative.
	Friction = FMath::Max(0.f, Friction);
	const auto MaxAccel = GetMaxAcceleration();
	auto MaxSpeed = GetMaxSpeed();

	// Check if path following requested movement
	auto bZeroRequestedAcceleration = true;
	auto RequestedAcceleration = FVector::ZeroVector;
	auto RequestedSpeed = 0.0f;
	if (ApplyRequestedMove(DeltaTime, MaxAccel, MaxSpeed, Friction, BrakingDeceleration, RequestedAcceleration, RequestedSpeed))
	{
		RequestedAcceleration = RequestedAcceleration.GetClampedToMaxSize(MaxAccel);
		bZeroRequestedAcceleration = false;
	}

	// Check if we are forcing the player to move at the max possible acceleration for the current move mode...
	// (aka walking automatically).
	if (bForceMaxAccel)
	{
		// Force acceleration at full speed.
		// In consideration order for direction: Acceleration, then Velocity, then Pawn's rotation.
		if (Acceleration.SizeSquared() > SMALL_NUMBER)
		{
			Acceleration = GetSafeNormalPrecise(Acceleration) * MaxAccel;
		}
		else
		{
			Acceleration = MaxAccel * (Velocity.SizeSquared() < SMALL_NUMBER ? UpdatedComponent->GetForwardVector() : GetSafeNormalPrecise(Velocity));
		}

		AnalogInputModifier = 1.f;
	}

	// Path following above didn't care about the analog modifier, but we do for everything else below, so get the fully modified value.
	// Use max of requested speed and max speed if we modified the speed in ApplyRequestedMove above.
	MaxSpeed = FMath::Max(RequestedSpeed, MaxSpeed * AnalogInputModifier);
	auto Speed = Velocity.Size();

	// Apply fluid friction if necessary
	if (bFluid)
	{
		Velocity *= (1.f - FMath::Min(Friction * DeltaTime, 1.f));
	}

	// Get size and direction of acceleration.
	const auto AccelDirection = GetSafeNormalPrecise(Acceleration);
	const auto AccelAmount = Acceleration.Size();

	// Apply ground friction first
	if (ShouldApplyGroundFriction())
	{
		ApplyFriction(DeltaTime, Friction, 1.0f, StopSpeed);
	}

	// Apply acceleration for movement.
	if (IsMovingOnGround())
	{
		ApplyAcceleration(DeltaTime, 1.0f, AccelDirection, MaxSpeed, AccelAmount);
	}
	else if (IsFalling())
	{
		ApplyAirAcceleration(DeltaTime, 1.0f, AccelDirection, MaxSpeed, MaxFallAirSpeed, AccelAmount);
	}

	// Apply path following acceleration & clamp speed to enforced speed limit if enabled (regardless if player is bhopping or strafing).
	Velocity += RequestedAcceleration * DeltaTime;
	if (bUseEnforcedMaxSpeed)
	{
		Velocity = GetClampedToMaxSizePrecise(Velocity, EnforcedMaxSpeed);
	}

	// Restore the current movement mode again.
	if (LastCalcVeloMovementMode == MOVE_Falling)
	{
		// NOTE Not using SetMovementMode() so that other code doesn't get notified.
		MovementMode = CurrentMovementMode;
	}
	LastCalcVeloMovementMode = MovementMode;

	// Check if RVO avoidance is enabled for AI movement & calculate the required avoidance velo from it.
	if (bUseRVOAvoidance)
	{
		CalcAvoidanceVelocity(DeltaTime);
	}
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	auto Speed = 0.0f;

	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		// NOTE Falling uses the walking speed, as MaxAirSpeed is used seperately by ApplyAirAcceleration()
	case MOVE_Falling:
		Speed = (IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed);
		break;

	case MOVE_Swimming:
		Speed = MaxSwimSpeed;
		break;

	case MOVE_Flying:
		Speed = MaxFlySpeed;
		break;

	case MOVE_Custom:
		Speed = MaxCustomMovementSpeed;
		break;

	case MOVE_None:
	default:
		Speed = 0.f;
		break;
	}

	return Speed;
}

float UBaseCharacterMovementComponent::GetMaxAcceleration() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
	case MOVE_Swimming:
	case MOVE_Custom:
		return MaxAcceleration;

	case MOVE_Falling:
	case MOVE_Flying:
		return MaxAirAcceleration;

	case MOVE_None:
	default:
		return 0.f;
	}
}
