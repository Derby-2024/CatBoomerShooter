// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCollectible.h"

FCollectibles::FCollectibles()
{
	Cats.CatCount = 0;
	Cats.TotalCats = 10;
	Cats.ItemInformation = "Cats";
}

FCollectibleStructX* FCollectibles::GetCollectionOfType(ECollectibleType CollectibleType)
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


// Sets default values
ABaseCollectible::ABaseCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCollectible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
