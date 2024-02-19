// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWhip.h"
#include "Kismet/GameplayStatics.h"
#include "CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "CatBoomerShooter/Character/BasePlayerInterface.h"
#include "CatBoomerShooter/Enemy/EnemyDamage.h"


// Sets default values
ABaseWhip::ABaseWhip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene Component");
	RootComponent = DefaultSceneComponent;

	Whip_SK = CreateDefaultSubobject<USkeletalMeshComponent>("Whip Skeletal Mesh");
	Whip_SK->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);

	AttackOneCollision = CreateDefaultSubobject<UBoxComponent>("Attack One Collider");
	AttackOneCollision->SetupAttachment(RootComponent);

	AttackTwoCollision = CreateDefaultSubobject<UBoxComponent>("Attack Two Collider");
	AttackTwoCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseWhip::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	USceneComponent* WhipLocation = IBasePlayerInterface::Execute_GetPlayerWhipLocation(PlayerCharacter);
	
	if(IsValid(WhipLocation))
	{
		this->AttachToComponent(WhipLocation, FAttachmentTransformRules::SnapToTargetIncludingScale, "GripPoint");
		//this->SetActorHiddenInGame(true);
	}
	
}

// Called every frame
void ABaseWhip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWhip::Attack()
{
	if(isAttacking)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	isAttacking = true;
	this->SetActorHiddenInGame(false);

	TArray<AActor*> Result = {};
	if (AttackCombo < 3)
	{
		AttackOneCollision->GetOverlappingActors(Result);
			for (AActor* actor : Result)
			{
				if (actor->ActorHasTag("Enemy"))
				{
					EnemiesHit.AddUnique(Cast<ACharacter>(actor));
				}
			}
	}
	else
	{
		AttackTwoCollision->GetOverlappingActors(Result);
			for (AActor* actor : Result)
			{
				if (actor->ActorHasTag("Enemy"))
				{
					EnemiesHit.AddUnique(Cast<ACharacter>(actor));
				}
			}
	}

	for (ACharacter* enemy : EnemiesHit)
	{
		IEnemyDamage::Execute_TakeHealthDamage(enemy, Damage);
		if (AttackCombo < 3)
		{
			LaunchVelocity = PlayerCharacter->GetActorForwardVector() * KnockbackPower;
		}
		else
		{
			LaunchVelocity = PlayerCharacter->GetActorForwardVector() * KnockbackPower * 5;
		}
		enemy->LaunchCharacter(LaunchVelocity, false, false);
	}
	UE_LOG(LogTemp, Warning, TEXT("DEBUG1"));
	if (EnemiesHit.IsEmpty())
	{
		AttackFinished();
		return;
	}
	AttackCombo++;
	if (AttackCombo > 3)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimer);
		ResetCombo();
	}
	else
	{
		if(GetWorldTimerManager().IsTimerActive(CountdownTimer))
		{
			GetWorldTimerManager().ClearTimer(CountdownTimer);
		}
		GetWorldTimerManager().SetTimer(CountdownTimer, this, &ABaseWhip::ResetCombo, 2.0f, false);
	}
	AttackFinished();

	//FTimerHandle TimerHandle;
    //GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseWhip::AttackFinished, 2.0f, false);
}

void ABaseWhip::AttackFinished()
{
	isAttacking = false;
	EnemiesHit = {};
	//this->SetActorHiddenInGame(true);
	UE_LOG(LogTemp, Warning, TEXT("Attack Ended"));
}

void ABaseWhip::ResetCombo()
{
	AttackCombo = 1;
}
