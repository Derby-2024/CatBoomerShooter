
#include "Train.h"
#include "Engine/DamageEvents.h"

// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);

    TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
    TrainMesh->SetupAttachment(SplineComponent);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnTrainOverlap);
    CollisionComponent->SetupAttachment(TrainMesh);

    TrainSpeed = 300.0f; 
    DamageAmount = 10.0f;
    PushForce = 2000.0f;

}

// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();

    SetNextTargetPoint();
    MoveTowardsTarget();
    bMovingForward = true;
}

// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    MoveTowardsTarget();
}

void ATrain::SetNextTargetPoint()
{
    // Get the location at the random input key along the spline
    float RandomInputKey = FMath::FRandRange(0.0f, 1.0f);
    NextTargetLocation = SplineComponent->GetLocationAtSplineInputKey(RandomInputKey, ESplineCoordinateSpace::World);
}

void ATrain::MoveTowardsTarget()
{
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = NextTargetLocation - CurrentLocation;
    Direction.Normalize();

    FVector NewLocation = CurrentLocation + Direction * TrainSpeed * GetWorld()->GetDeltaSeconds();

    SetActorLocation(NewLocation);

    // Check if the train has reached the target location
    float DistanceToTarget = FVector::Distance(NewLocation, NextTargetLocation);
    if (DistanceToTarget < 10.0f)
    {
        // Set a new target point
        SetNextTargetPoint();
    }
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