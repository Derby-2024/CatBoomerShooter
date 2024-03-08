// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickup.h"
#include "../../Character/BasePlayerCharacter.h"


AKeyPickup::AKeyPickup()
{
    // Set default value for ItemInfo
    ItemInformation = "KeyString";

}

void AKeyPickup::OnPickup()
{

}

void AKeyPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapped actor is the player character
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(OtherActor);
    
    // Check if the player character is valid
    if (PlayerCharacter)
    {
        // Access the player character's inventory component
        UInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
        if (InventoryComponent)
        {
            // Create a new item with the key type and item info
            FItem KeyItem;
            KeyItem.ItemType = EItemType::Key;
            KeyItem.ItemInfo = ItemInformation;

            // Add the key to the inventory
            if (InventoryComponent->AddItem(KeyItem))
            {
                Destroy();
            }
        }
    }
}
