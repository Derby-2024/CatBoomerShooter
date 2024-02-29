// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAmmo.h"

// Sets default values
ABaseAmmo::ABaseAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set default values for Assualt Rifle
	FAmmoStruct AssualtRifleAmmo;
	AssualtRifleAmmo.AmmoAmount = 30;
	AssualtRifleAmmo.TotalAmmo = 65;
	AssualtRifleAmmo.AmmoType = EAmmoType::E_AssaultRifle;

	//Set default values for Shotgun
	FAmmoStruct ShotgunAmmo;
	ShotgunAmmo.AmmoAmount = 8;
	ShotgunAmmo.TotalAmmo = 8;
	ShotgunAmmo.AmmoType = EAmmoType::E_Shotgun;

	//Set default values for SniperRifle
	FAmmoStruct SniperRifleAmmo;
	SniperRifleAmmo.AmmoAmount = 16;
	SniperRifleAmmo.TotalAmmo = 8;
	SniperRifleAmmo.AmmoType = EAmmoType::E_SniperRifle;

	//Set default values for Pistol
	FAmmoStruct PistolAmmo;
	PistolAmmo.AmmoAmount = 8;
	PistolAmmo.TotalAmmo = 16;
	PistolAmmo.AmmoType = EAmmoType::E_Pistol;

	//Set default values for Grenade
	FAmmoStruct GrenadeAmmo;
	GrenadeAmmo.AmmoAmount = 2;
	GrenadeAmmo.TotalAmmo = 4;
	GrenadeAmmo.AmmoType = EAmmoType::E_Grenade;
}

// Called when the game starts or when spawned
void ABaseAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

