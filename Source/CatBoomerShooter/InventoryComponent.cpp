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
    switch (Item.ItemType)
    {
    case EItemType::Ammo:
        for (FAmmoStruct& AmmoItem :Ammo)
        {
            if (AmmoItem.AmmoType == Item.AmmoType)
            {
                if (AmmoItem.AmmoAmount < AmmoItem.TotalAmmo)
                {
                    // (clamped to max ammo)
                    AmmoItem.AmmoAmount += Item.AmmoAmount;
                    AmmoItem.AmmoAmount = FMath::Clamp(AmmoItem.AmmoAmount, 0, AmmoItem.TotalAmmo);
                    return true;
                }
                else 
                {
                    return false;
                }
            }
        }
        break;

    case EItemType::Weapon:
        
        break;

    case EItemType::Health:
        
        break;

    case EItemType::Collectible:
        
        break;
    default:
        // Debug message indicating no logic for this specific item type
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
        for (FAmmoStruct& AmmoItem : Ammo)
        {
            if (AmmoItem.AmmoType == Item.AmmoType)
            {
                // Check if the amount to remove is less than or equal to the available ammo amount
                if (Item.AmmoAmount <= AmmoItem.AmmoAmount)
                {
                    AmmoItem.AmmoAmount -= Item.AmmoAmount;
                    return true;
                }
                else
                {
                 // If trying to remove more ammo than available, remove all available and return false
                    AmmoItem.AmmoAmount = 0;
                    return false;
                }
            }
        }
        break;

    case EItemType::Weapon:
        
        break;

    case EItemType::Health:

        break;

    case EItemType::Collectible:
    
        break;

    default:
        // Debug message indicating no logic for this specific item type
        UE_LOG(LogTemp, Warning, TEXT("No logic for removing item of type %s"), *UEnum::GetValueAsString(Item.ItemType));
        break;
    }

    // Return false if the removal failed
    return false;
}

