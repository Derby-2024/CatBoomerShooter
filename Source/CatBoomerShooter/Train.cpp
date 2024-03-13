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
    MoveTowardsTarget();
}

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
        float DistanceAlongSpline = SplineComponent->GetDistanceAlongSplineAtSplinePoint(CurrentSplineIndex);
        //Get the rotation at the calculated distance along the spline
        FRotator SplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

        //Calculate movement direction
        FVector Direction = NextTargetLocation - CurrentLocation;
        Direction.Normalize();
        FVector NewLocation = CurrentLocation + Direction * TrainSpeed * GetWorld()->GetDeltaSeconds();
        TrainMesh->SetWorldLocation(NewLocation);
    }
}

void ATrain::TeleportTrain()
{
    //Teleport the train back to the start
    UE_LOG(LogTemp, Warning, TEXT("Teleported!"));
    TrainMesh->SetWorldLocation(StartLocation);
    SetNextTargetPoint();

    bIsTeleportTimerActive = false;
}

void ATrain::OnTrainOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        PlayerCharacter->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);

        FVector PushDirection = -SweepResult.ImpactNormal;

        PlayerCharacter->LaunchCharacter(PushDirection * PushForce, true, true);
    }
}