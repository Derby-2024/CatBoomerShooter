#include "Train.h"
#include "Engine/DamageEvents.h"

ATrain::ATrain()
{
    PrimaryActorTick.bCanEverTick = true;

    TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
    RootComponent = TrainMesh;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnTrainOverlap);
    CollisionComponent->SetupAttachment(RootComponent);

    CurrentSplineIndex = 0;
    bIsTeleportTimerActive = false;

    DamageAmount = 10.0f;
    PushForce = 2000.0f;

    MinRandomDelay = 1.0f;
    MaxRandomDelay = 8.0f;
}

void ATrain::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    SetNextTargetPoint();
}

void ATrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Rename function to MoveAlongSpline
    MoveTowardsTarget();
}

// This we can remove
void ATrain::SetNextTargetPoint()
{
    //CurrentSplineIndex stays within valid range
    if (CurrentSplineIndex >= SplineComponent->GetNumberOfSplinePoints() - 1)
    {
        CurrentSplineIndex = 0; //Reset to the start if reached the end
    }
    else
    {
        CurrentSplineIndex++;
    }
        
    NextTargetLocation = SplineComponent->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
}

void ATrain::MoveTowardsTarget()
{
    FVector CurrentLocation = TrainMesh->GetComponentLocation();
    float DistanceToTarget = FVector::Distance(CurrentLocation, NextTargetLocation);


    // Check if out current distance variable is higher than spline lenght
    // if so we reset distance variable and teleport train mesh back to 0.

    //If the train has reached the target spline point and teleportation timer is not active
    if (DistanceToTarget < TrainSpeed * GetWorld()->GetDeltaSeconds() && !bIsTeleportTimerActive)
    {
        //Stop the train
        TrainMesh->SetWorldLocation(NextTargetLocation);

        //Check if train has reached the end of the spline point
        if (CurrentSplineIndex == SplineComponent->GetNumberOfSplinePoints() - 1)
        {
            UE_LOG(LogTemp, Warning, TEXT("Set Timer Delay"));

            //Calculate a random delay
            RandomDelay = FMath::FRandRange(MinRandomDelay, MaxRandomDelay);

            //Start a timer to teleport the train
            GetWorld()->GetTimerManager().SetTimer(TeleportDelay, this, &ATrain::TeleportTrain, RandomDelay, false);

            //Set timer active
            bIsTeleportTimerActive = true;

            //Generate a random speed after teleportation
            TrainSpeed = FMath::RandRange(900.0f, 2500.0f);

            return;
        }
        else
        {
            SetNextTargetPoint();
        }

        return;
    }
    else
    {

        ////Way in the video 
        //float DistanceAlongSpline = SplineComponent->GetDistanceAlongSplineAtSplinePoint(CurrentSplineIndex) + TrainSpeed;
        //FVector Direction = NextTargetLocation - CurrentLocation;
        //Direction.Normalize();
        //FVector NewLocation = CurrentLocation + Direction * TrainSpeed * GetWorld()->GetDeltaSeconds();
        //TrainMesh->SetWorldLocation(NewLocation);

        //FTransform TransformDistanceAlongSpline = SplineComponent->GetTransformAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World); 

        //FVector Location = TransformDistanceAlongSpline.GetLocation();
        //FRotator Rotation = TransformDistanceAlongSpline.Rotator();

        //float LocationX = Location.X;
        //float LocationY = Location.Y;
        //float LocationZ = Location.Z + (-89.0f);

        //float RotationPitch = Rotation.Pitch;
        //float RotationYaw = Rotation.Yaw + (-90.0f);
        //float RotationRoll = Rotation.Roll;

        //TrainMesh->SetRelativeLocationAndRotation(FVector(LocationX, LocationY, LocationZ), FRotator(RotationPitch, RotationYaw, RotationRoll)); 


        //Way that somewhat works
        FTransform TransformDistanceAlongSpline = SplineComponent->GetTransformAtDistanceAlongSpline(DistanceToTarget, ESplineCoordinateSpace::World);

        //Calculate movement direction
        FVector Direction = NextTargetLocation - CurrentLocation; 
        Direction.Normalize(); 

        float DistanceAlongSpline = SplineComponent->GetDistanceAlongSplineAtSplinePoint(CurrentSplineIndex); 
        FVector Location = CurrentLocation + Direction * TrainSpeed * TransformDistanceAlongSpline.GetLocation(); 

        //FVector NewLocation = CurrentLocation + Direction * TrainSpeed * GetWorld()->GetDeltaSeconds();

        TrainMesh->SetWorldLocation(Location); 

        //Calculate rotation based on the tangent of the spline
        FVector Tangent = SplineComponent->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
        FRotator NewRotation = Tangent.Rotation(); 

        //Set the train's new rotation
        TrainMesh->SetRelativeRotationExact(FRotator(NewRotation.Yaw));
    }
}

void ATrain::TeleportTrain()
{
    // Teleport train back to index 0
    // stop the train (boolean that is check on tick when trying to move)
    // Start time to start train again.


    //Teleport the train back to the start
    UE_LOG(LogTemp, Warning, TEXT("Teleported!"));
    TrainMesh->SetWorldLocation(StartLocation);
    SetNextTargetPoint();       // instead of setting next target, we teleport the train to spline index 0 location.

    bIsTeleportTimerActive = false;
}

void ATrain::OnTrainOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (PlayerCharacter)
    {
        PlayerCharacter->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);

        FVector PushDirection = -SweepResult.ImpactNormal;

        PlayerCharacter->LaunchCharacter(PushDirection * PushForce, true, true);
    }
}