// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAmmo.h"


FAmmoCollection::FAmmoCollection()
{
	ShotgunAmmo.AmmoAmount = 8;
	ShotgunAmmo.TotalAmmo = 8;

	AssaultRifleAmmo.AmmoAmount = 30;
	AssaultRifleAmmo.TotalAmmo = 65;

	GrenadeAmmo.AmmoAmount = 1;
	GrenadeAmmo.TotalAmmo = 3;

	PistolAmmo.AmmoAmount = 6;
	PistolAmmo.TotalAmmo = 12;

	SniperAmmo.AmmoAmount = 3;
	SniperAmmo.TotalAmmo = 6;
}

FAmmoStruct* FAmmoCollection::GetCollectionOfType(EAmmoType AmmoType)
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
