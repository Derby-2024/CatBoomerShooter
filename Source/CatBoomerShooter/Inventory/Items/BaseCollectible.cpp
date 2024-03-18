// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCollectible.h"

FCollectibles::FCollectibles()
{
	Cats.CatCount = 0;
	Cats.TotalCats = 10;
	Cats.ItemInformation = "Cats";
}

FCollectibleData* FCollectibles::GetCollectionOfType(ECollectibleType CollectibleType)
{
	UE_LOG(LogTemp, Log, TEXT("Getting collection"));

	switch (CollectibleType)
	{
	case ECollectibleType::E_Cat:
		return &Cats;

	default:
		return nullptr;
	}

}
