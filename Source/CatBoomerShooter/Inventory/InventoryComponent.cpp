// Fill out your copyright notice in the Description page of Project Settings.
#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

bool UInventoryComponent::AddItem(const FItem& Item)
{
    UE_LOG(LogTemp, Log, TEXT("AddItem"));

    switch (Item.ItemType)
    {
    case EItemType::Ammo: {

        FAmmoStructX* Collection = Ammo.GetCollectionOfType(Item.AmmoType);

        UE_LOG(LogTemp, Log, TEXT("AddAmmo, %d, %d, %d"), Item.AmmoAmount, Collection->AmmoAmount, Collection->TotalAmmo);

        if (Collection->AmmoAmount + Item.AmmoAmount > Collection->TotalAmmo)
        {
            UE_LOG(LogTemp, Warning, TEXT("Maximum ammo capacity reached, cannot pick up more ammo."));
            return false;
        }

        Collection->AmmoAmount += Item.AmmoAmount;
        UE_LOG(LogTemp, Log, TEXT("AddAmmo, %d, %d, %d"), Item.AmmoAmount, Collection->AmmoAmount, Collection->TotalAmmo);
        return true;
    }
    case EItemType::Weapon:

        break;

    case EItemType::Health:

        break;

    case EItemType::Collectible:
    {
        if (FCollectibleStructX* Collection = Collectibles.GetCollectionOfType(Item.CollectibleType)) {
            Collection->CatCount += Item.CatCount;

            return true;
        }
    }

    case EItemType::Key:
    {
        Keys.Add(Item.ItemInfo);
        return true;
    }

    default:
        UE_LOG(LogTemp, Warning, TEXT("No logic for adding item of type %s"), *UEnum::GetValueAsString(Item.ItemType));
        break;
    }

    // Return false if the addition failed
    return false;
}

bool UInventoryComponent::RemoveItem(const FItem& Item)
{
    switch (Item.ItemType)
    {
    case EItemType::Ammo:

        break;

    case EItemType::Weapon:

        break;

    case EItemType::Health:

        break;

    case EItemType::Collectible:

        break;

    case EItemType::Key:

        break;

    default:
        // Debug message indicating no logic for this specific item type
        UE_LOG(LogTemp, Warning, TEXT("No logic for removing item of type %s"), *UEnum::GetValueAsString(Item.ItemType));
        break;
    }

    // Return false if the removal failed
    return false;
}