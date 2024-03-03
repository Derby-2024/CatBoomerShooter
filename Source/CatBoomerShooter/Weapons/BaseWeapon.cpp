// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "CatBoomerShooter/Character/BasePlayerInterface.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene Component");
	RootComponent = DefaultSceneComponent;

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(RootComponent);

	TotalAmmo = 45;
	AmmoAmount = 5;
	CurrentAmmo = 45;
	ReloadTime = 1.0f;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
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
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::StartShooting()
{
	switch(FiringMode)
	{
		case(EFiringMode::Semi):
			Fire();
			return;
		case(EFiringMode::Automatic):
			Fire();
			GetWorldTimerManager().SetTimer(Handle_ReFire, this, &ABaseWeapon::Fire, 60.0f / FireRate, true);
			return;
		case(EFiringMode::Burst):
			if (!GetWorldTimerManager().IsTimerActive(Handle_ReFire))
			{
				ShotsFired = 0;
				Fire();
				GetWorldTimerManager().SetTimer(Handle_ReFire, this, &ABaseWeapon::Fire, 60.0f / FireRate, true);
			}
			return;
	}
	
}

void ABaseWeapon::StopShooting()
{
	if (FiringMode != EFiringMode::Burst)
	{
		GetWorldTimerManager().ClearTimer(Handle_ReFire);
	}
}

void ABaseWeapon::Fire()
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
	FTransform SpawnTransform = WeaponMesh->GetSocketTransform("Muzzle", RTS_World);
	ABaseWeaponProjectile* Projectile = World->SpawnActor<ABaseWeaponProjectile>(ProjectileClass, SpawnTransform, SpawnParams);
	if (FiringMode == EFiringMode::Burst)
	{
		ShotsFired++;
		if (ShotsFired >= BurstAmount)
		{
			GetWorldTimerManager().ClearTimer(Handle_ReFire);
		}
	}
}

