// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWhip.h"
#include "Kismet/GameplayStatics.h"
#include "CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "CatBoomerShooter/Character/BasePlayerInterface.h"

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

	AttackOneCollision = CreateDefaultSubobject<UBoxComponent>("Attack Collider");
	AttackOneCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseWhip::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	USkeletalMeshComponent* Arms = IBasePlayerInterface::Execute_GetPlayerArms(PlayerCharacter);
	
	if(IsValid(Arms))
	{
		this->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, "R_HandSocket");
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
	UE_LOG(LogTemp, Warning, TEXT("BaseWhip: Attacking"));
	isAttacking = true;
	this->SetActorHiddenInGame(false);

	TArray<AActor*> Result = {};
	AttackOneCollision->GetOverlappingActors(Result);
	for (AActor* actor : Result)
	{
		if (actor->ActorHasTag("Enemy"))
		{
			EnemiesHit.AddUnique(Cast<ACharacter>(actor));
		}
	}

	for (ACharacter* enemy : EnemiesHit)
	{
		AController* MyInstigator = GetInstigatorController();
		UClass* DamageType = UDamageType::StaticClass();

		UGameplayStatics::ApplyDamage(enemy, Damage, MyInstigator, this, DamageType);

		if (!hasBigKnockback)
		{
			LaunchVelocity = PlayerCharacter->GetActorForwardVector() * KnockbackPower;
		}
		else
		{
			LaunchVelocity = PlayerCharacter->GetActorForwardVector() * KnockbackPower * 5;
			usedBigKnockback = true;
		}
		enemy->LaunchCharacter(LaunchVelocity, false, false);
	}

	if (usedBigKnockback)
	{
		hasBigKnockback = false;
		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ABaseWhip::ResetCombo, 10.0f, false);
		usedBigKnockback = false;
	}

	isAttacking = false;
	EnemiesHit = {};
	//this->SetActorHiddenInGame(true);
	UE_LOG(LogTemp, Warning, TEXT("BaseWhip: Attack Ended"));
}

void ABaseWhip::ResetCombo()
{
	hasBigKnockback = true;
	UE_LOG(LogTemp, Warning, TEXT("BaseWhip: Attack Reset"));
}
