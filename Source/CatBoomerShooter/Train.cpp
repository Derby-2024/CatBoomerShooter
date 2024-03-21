#include "Train.h"
#include "Engine/DamageEvents.h"

ATrain::ATrain()
{
    PrimaryActorTick.bCanEverTick = true;

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    RootComponent = SplineComponent;

    TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
    TrainMesh->SetupAttachment(RootComponent);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnTrainOverlap);
    CollisionComponent->SetupAttachment(TrainMesh);

    CurrentDistance = 0;
    bIsTeleportTimerActive = false;

    DamageAmount = 10.0f;
    PushForce = 2000.0f;

    MinSpeed = 2000.0f;
    MaxSpeed = 3500.0f;

    MinRandomDelay = 1.0f;
    MaxRandomDelay = 8.0f;
}

void ATrain::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
}

void ATrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MoveAlongSpline();
}

void ATrain::MoveAlongSpline()
{

    if (bIsTeleportTimerActive)
    {
        return;
    }

    //Get the length of the spline
    float SplineLength = SplineComponent->GetSplineLength();
    if (CurrentDistance >= SplineLength)
    {
        TeleportTrain();
        return;
    }
    else
    {
        //Calculate the distance to move based on speed and delta time
        float DeltaDistance = TrainSpeed * GetWorld()->GetDeltaSeconds();

        //Update the current distance
        CurrentDistance += DeltaDistance;

        //If the new distance exceeds spline length, teleport train
        if (CurrentDistance >= SplineLength)
        {
            TeleportTrain();
            return;
        }

        //Get the transform at the new distance along the spline
        FTransform NewTransform = SplineComponent->GetTransformAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
        NewTransform.SetScale3D(TrainMesh->GetComponentScale());

        //Set the world transform of the train mesh
        TrainMesh->SetWorldTransform(NewTransform);
    }
}

void ATrain::TeleportTrain()
{
    bIsTeleportTimerActive = true;

    //Get the transform at spline point 0
    FTransform SplineTransform = SplineComponent->GetTransformAtSplinePoint(0, ESplineCoordinateSpace::World);
    SplineTransform.SetScale3D(TrainMesh->GetComponentScale());
    TrainMesh->SetWorldTransform(SplineTransform);
   
    TrainSpeed = FMath::RandRange(MinSpeed, MaxSpeed);

    CurrentDistance = 0;

    //Start the timer to resume train movement
    RandomDelay = FMath::RandRange(MinRandomDelay, MaxRandomDelay); 
    GetWorld()->GetTimerManager().SetTimer(TeleportDelay, this, &ATrain::ResumeTrain, RandomDelay, false); 
}

void ATrain::ResumeTrain()
{
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