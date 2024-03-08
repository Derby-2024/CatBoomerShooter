// Fill out your copyright notice in the Description page of Project Settings.
#include "InventoryComponent.h"

DEFINE_LOG_CATEGORY(LogInventory);

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
    switch (Item.ItemType)
    {
    case EItemType::Ammo:
    {
        FAmmoStruct* Collection = Ammo.GetCollectionOfType(Item.AmmoType);
        UE_LOG(LogInventory, Log, TEXT("AddAmmo, %d, %d, %d"), Item.AmmoAmount, Collection->AmmoAmount, Collection->TotalAmmo);
        
        // Calculate the remaining ammo needed to reach the totalammo
        int RemainingAmmo = Collection->TotalAmmo - Collection->AmmoAmount;


        if (Collection->AmmoAmount + Item.AmmoAmount > Collection->TotalAmmo)
        {
            UE_LOG(LogInventory, Warning, TEXT("Maximum ammo capacity reached, cannot pick up more ammo."));
            return false;
        }

        // Add the minimum between remaining ammo
        int AddedAmmo = FMath::Min(Item.AmmoAmount, RemainingAmmo);

        // Update the collection's ammo amount
        Collection->AmmoAmount += AddedAmmo;
        UE_LOG(LogInventory, Log, TEXT("Added %d ammo. New ammo amount: %d"), AddedAmmo, Collection->AmmoAmount);

        if (AddedAmmo < Item.AmmoAmount)
        {
            UE_LOG(LogInventory, Warning, TEXT("Maximum ammo capacity reached, could only pick up %d of %d ammo."), AddedAmmo, Item.AmmoAmount);
            return false;
        }

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
        UE_LOG(LogInventory, Warning, TEXT("No logic for adding item of type %s"), *UEnum::GetValueAsString(Item.ItemType));
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
    {
        FAmmoStruct* Collection = Ammo.GetCollectionOfType(Item.AmmoType);

        // Check if the amount to remove is valid (not negative)
        if (Collection->AmmoAmount >= Item.AmmoAmount)
        {
            // Subtract ammo
            Collection->AmmoAmount = FMath::Max(Collection->AmmoAmount - Item.AmmoAmount, 0);

            UE_LOG(LogInventory, Log, TEXT("Removed %d ammo. New ammo amount: %d"), Item.AmmoAmount, Collection->AmmoAmount);

            return true;
        }
        break;
    }

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
        UE_LOG(LogInventory, Warning, TEXT("No logic for removing item of type %s"), *UEnum::GetValueAsString(Item.ItemType));
        break;
    }
    
    // Return false if the removal failed
    return false;
}