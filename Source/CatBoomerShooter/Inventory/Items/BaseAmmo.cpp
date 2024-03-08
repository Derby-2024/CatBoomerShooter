// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAmmo.h"


FAmmoCollection::FAmmoCollection()
{
	ShotgunAmmo.AmmoAmount = 8;
	ShotgunAmmo.MaxAmmo = 8;

	AssaultRifleAmmo.AmmoAmount = 30;
	AssaultRifleAmmo.MaxAmmo = 65;

	GrenadeAmmo.AmmoAmount = 1;
	GrenadeAmmo.MaxAmmo = 3;

	PistolAmmo.AmmoAmount = 6;
	PistolAmmo.MaxAmmo = 12;

	SniperAmmo.AmmoAmount = 3;
	SniperAmmo.MaxAmmo = 6;
}

FAmmoContainer* FAmmoCollection::GetCollectionOfType(EAmmoType AmmoType)
{
	switch (AmmoType)
	{
	case EAmmoType::E_AssaultRifle:
		return &AssaultRifleAmmo;
	case EAmmoType::E_Grenade:
		return &GrenadeAmmo;
	case EAmmoType::E_Shotgun:
		return &ShotgunAmmo;
	case EAmmoType::E_SniperRifle:
		return &SniperAmmo;
	case EAmmoType::E_Pistol:
		return &PistolAmmo;

	default:
		return nullptr;
	}
}
