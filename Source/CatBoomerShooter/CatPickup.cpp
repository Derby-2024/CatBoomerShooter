// Fill out your copyright notice in the Description page of Project Settings.


#include "CatPickup.h"
#include "Character/BasePlayerCharacter.h"
#include "BaseCollectible.h"

ACatPickup::ACatPickup()
{
    Quantity = 1;
    CollectibleType = ECollectibleType::E_Cat;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    RootComponent = CollisionComponent;

    // Bind the overlap function
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACatPickup::OnOverlap);
}

void ACatPickup::OnPickup()
{
        Super::OnPickup();

        // Check if the actor that overlapped is the player character
        ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
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
                InventoryComponent->AddItem(CatItem);
                Destroy();
            }
        }
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

            if (InventoryComponent->AddItem(CatItem))
            {
                Destroy();
            }
        }
    }
}

void ACatPickup::RunawaySequence(AActor* BasePlayerCharacter)
{

}
