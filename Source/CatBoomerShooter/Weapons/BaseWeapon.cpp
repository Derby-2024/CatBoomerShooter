// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "../Inventory/Items/BaseAmmo.h"
#include "../Character/BasePlayerCharacter.h"
#include "../Inventory/InventoryComponent.h"
#include "CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "CatBoomerShooter/Character/BasePlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"

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

	AmmoType = EAmmoType::E_AssaultRifle;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon::BeginPlay: Cannot get owner!"));
		return;
	}

	OwningCharacter = Cast<APawn>(GetOwner());
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon::BeginPlay: Cannot cast to pawn"));
		return;
	}

	if (OwningCharacter->Implements<UBasePlayerInterface>() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWeapon::BeginPlay: Owner Doesn't Implement Interface!"));
		return;
	}

	USkeletalMeshComponent* Arms = IBasePlayerInterface::Execute_GetPlayerArms(OwningCharacter);
	if(IsValid(Arms))
	{
		this->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
		//this->SetActorHiddenInGame(true);
	}

	for (FName SocketName : WeaponMesh->GetAllSocketNames())
	{
		if (SocketName.ToString().StartsWith("Muzzle"))
		{
			MuzzleSockets.AddUnique(SocketName);
		}
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

	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Check if PlayerCharacter is valid
	if (Player)
	{
		UInventoryComponent* InventoryComponent = Player->GetInventoryComponent();
		if (InventoryComponent)
		{
			// Check if there is enough ammo
			FItem CurrentAmmo;
			CurrentAmmo.ItemType = EItemType::Ammo;
			CurrentAmmo.AmmoAmount = 1;
			CurrentAmmo.AmmoType = AmmoType;

			if (!InventoryComponent->RemoveItem(CurrentAmmo))
			{
				UE_LOG(LogTemp, Warning, TEXT("BaseWeapon::Fire: Not enough ammo to fire!"));
				StopShooting();
				return;
			}

			FAmmoContainer* Collection = InventoryComponent->Ammo.GetCollectionOfType(AmmoType);
			if (Collection && Collection->AmmoAmount <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("BaseWeapon::Fire: Not enough ammo to fire!"));
				StopShooting();
				return;
			}
		}
	}

	FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

	FHitResult Hit;

	if (OwningCharacter->IsPlayerControlled())
	{
		TraceStart = UGameplayStatics::GetPlayerCameraManager(World, 0)->GetCameraLocation();
		TraceEnd = TraceStart + UGameplayStatics::GetPlayerCameraManager(World, 0)->GetActorForwardVector() * 5000;
	}
	else
	{
		TraceStart = OwningCharacter->GetActorLocation();
		TraceEnd = TraceStart + OwningCharacter->GetActorForwardVector() * 5000;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwningCharacter);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);

	for (FName Muzzle : MuzzleSockets)
	{
		for (int i = 1; i <= NumberOfPellets; i++)
		{
			FRotator SpawnRotation;
			FVector SpawnLocation;
			WeaponMesh->GetSocketWorldLocationAndRotation(Muzzle, SpawnLocation, SpawnRotation);

			if (Hit.bBlockingHit)
			{
				SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, Hit.ImpactPoint);
			}
			else
			{
				SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, Hit.TraceEnd);
			}

			FTransform SpawnTransform = FTransform(RandomSpread(SpawnRotation, MaxVerticalSpread, MaxHorizontalSpread), SpawnLocation);

			ABaseWeaponProjectile* Projectile = World->SpawnActor<ABaseWeaponProjectile>(ProjectileClass, SpawnTransform, SpawnParams);

		}
	}
	if (FiringMode == EFiringMode::Burst)
	{
		ShotsFired++;
		if (ShotsFired >= BurstAmount)
		{
			GetWorldTimerManager().ClearTimer(Handle_ReFire);
		}
	}

	if (needsReload)
	{
		Reload();
	}
}

void ABaseWeapon::Reload()
{
	//Play Reload Animation
	UE_LOG(LogTemp, Warning, TEXT("BaseWeapon::Reload: Reload Animation Placeholder!"));
}

FRotator ABaseWeapon::RandomSpread(FRotator spawnRotation, float maxVertical, float maxHorizontal)
{
	float h_Spread = UKismetMathLibrary::RandomFloatInRange(-maxHorizontal, maxHorizontal);
	float v_Spread = UKismetMathLibrary::RandomFloatInRange(-maxVertical, maxVertical);

	return UKismetMathLibrary::ComposeRotators(FRotator(h_Spread, v_Spread, 0.0f), spawnRotation);
}