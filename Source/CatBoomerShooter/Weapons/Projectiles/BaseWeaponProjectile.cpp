// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeaponProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseWeaponProjectile::ABaseWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");
	BulletMesh->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeaponProjectile::OnOverlap);
	RootComponent = BulletMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->SetUpdatedComponent(BulletMesh);
	ProjectileMovement->InitialSpeed = 10000.0f;
    ProjectileMovement->MaxSpeed = 10000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABaseWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeaponProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) {
		UE_LOG(LogTemp, Warning, TEXT("ABaseWeaponProjectile::OnHit - Projectile %s has no owner."), *UKismetSystemLibrary::GetDisplayName(this))
		return;
	}

	AController* MyInstigator = GetInstigatorController();
	UClass* DamageType = UDamageType::StaticClass();


	if (OtherActor && OtherActor != this && OtherActor != MyOwner && OtherActor != MyOwner->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *UKismetSystemLibrary::GetDisplayName(OtherActor))
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyInstigator, this, DamageType);
		Destroy();
	}
}
