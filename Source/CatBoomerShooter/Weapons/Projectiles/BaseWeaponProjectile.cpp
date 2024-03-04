// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponProjectile.h"
#include "CatBoomerShooter/Enemy/DamageInterface.h"

// Sets default values
ABaseWeaponProjectile::ABaseWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");
	BulletMesh->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	BulletMesh->OnComponentHit.AddDynamic(this, &ABaseWeaponProjectile::OnHit);
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

void ABaseWeaponProjectile::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
	if (!OtherActor) return;

	if(OtherActor->GetClass()->ImplementsInterface(UDamageInterface::StaticClass()))
	{
		IDamageInterface::Execute_TakeHealthDamage(OtherActor, Damage);
	}
	Destroy();
}
