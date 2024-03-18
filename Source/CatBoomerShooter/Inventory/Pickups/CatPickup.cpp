// Fill out your copyright notice in the Description page of Project Settings.


#include "CatPickup.h"
#include "../../Character/BasePlayerCharacter.h"
#include "../Items/BaseCollectible.h"

ACatPickup::ACatPickup()
{
    Quantity = 1;
    CollectibleType = ECollectibleType::E_Cat;
}

void ACatPickup::OnPickup()
{

}

void ACatPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        // Get the inventory component from the player character
        UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
        if (InventoryComponent)
        {
            FItem CatItem;
            CatItem.ItemType = EItemType::Collectible;
            CatItem.CatCount = Quantity;
            CatItem.CollectibleInfo = ItemInformation;
            CatItem.CollectibleType = CollectibleType;

            if (InventoryComponent->AddItem(CatItem))
            {
                bIsDisabled = true;
                CheckActorDisabled();
            }
        }
    }
}

// Part of collectibles system to be later implemented
void ACatPickup::RunawaySequence(AActor* BasePlayerCharacter)
{

}
