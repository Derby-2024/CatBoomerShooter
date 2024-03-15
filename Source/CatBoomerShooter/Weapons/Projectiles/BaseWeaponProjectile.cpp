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

	BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeaponProjectile::OnOverlap);
	BulletMesh->OnComponentHit.AddDynamic(this, &ABaseWeaponProjectile::OnHit);

	//Add Ignores to weapon and its holder
	BulletMesh->IgnoreActorWhenMoving(GetOwner(), true);
	BulletMesh->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
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
		UE_LOG(LogTemp, Warning, TEXT("Overlapped %s"), *UKismetSystemLibrary::GetDisplayName(OtherActor))
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyInstigator, this, DamageType);
		Destroy();
	}
}

// Hit even on is triggered when hitting a static or dynamic objects so we only need to destroy the actor
// We only want to generate overlap events with characters as it should not push or apply physics on them
void ABaseWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *UKismetSystemLibrary::GetDisplayName(OtherActor))
	Destroy();
}
