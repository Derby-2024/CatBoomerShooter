
#include "Train.h"
#include "Engine/DamageEvents.h"

ATrain::ATrain()
{
	PrimaryActorTick.bCanEverTick = true;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);

    TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
    TrainMesh->SetupAttachment(SplineComponent);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnTrainOverlap);
    CollisionComponent->SetupAttachment(TrainMesh);
 
    DamageAmount = 10.0f;
    PushForce = 2000.0f;
}

void ATrain::BeginPlay()
{
    Super::BeginPlay();

    //Start location
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
    float EndInputKey = SplineComponent->GetNumberOfSplinePoints() - 1;
    NextTargetLocation = SplineComponent->GetLocationAtSplineInputKey(EndInputKey, ESplineCoordinateSpace::World);

    // Generate a random speed
    TrainSpeed = FMath::RandRange(900.0f, 2500.0f);
}

void ATrain::MoveTowardsTarget()
{
    FVector CurrentLocation = GetActorLocation();
    // Check if the train has reached the end of the spline
    float DistanceToTarget = FVector::Distance(CurrentLocation, NextTargetLocation);
    if (DistanceToTarget < 1.0f)
    {
        // Teleport the train back to the start
        UE_LOG(LogTemp, Warning, TEXT("Teleported!"));
        SetActorLocation(StartLocation);
        SetNextTargetPoint();
        return;
    }

    FVector Direction = NextTargetLocation - CurrentLocation;
    Direction.Normalize();
    FVector NewLocation = CurrentLocation + Direction * TrainSpeed * GetWorld()->GetDeltaSeconds();
    SetActorLocation(NewLocation);
}

void ATrain::OnTrainOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        PlayerCharacter->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);

        // Calculate the direction from the train to the player
        FVector PushDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
        PushDirection.Normalize();

        PlayerCharacter->LaunchCharacter(PushDirection * PushForce, true, true);
    }
}