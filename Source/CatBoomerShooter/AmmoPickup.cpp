#include "AmmoPickup.h"
#include "Character/BasePlayerCharacter.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"

AAmmoPickup::AAmmoPickup()
{
    // Set default values
    Quantity = 10;
    AmmoType = EAmmoType::E_AssaultRifle;

    // Create the collision component
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f); 
    RootComponent = CollisionComponent;

    // Bind the overlap function
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlap);
}

void AAmmoPickup::OnPickup()
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
            // Create an ammo item to add to the inventory
            FItem AmmoItem;
            AmmoItem.ItemType = EItemType::Ammo;
            AmmoItem.AmmoAmount = Quantity;
            AmmoItem.TotalAmmo = Quantity;
            AmmoItem.AmmoType = AmmoType;

            // Add the ammo item to the inventory
            InventoryComponent->AddItem(AmmoItem);
            Destroy();
        }
    }
}

void AAmmoPickup::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapped actor is the player character
    ABasePlayerCharacter* PlayerCharacter = Cast<ABasePlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        // Get the inventory component from the player character
        UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
        if (InventoryComponent)
        {
            // Create an ammo item to add to the inventory
            FItem AmmoItem;
            AmmoItem.ItemType = EItemType::Ammo;
            AmmoItem.AmmoAmount = Quantity;
            AmmoItem.TotalAmmo = Quantity;
            AmmoItem.AmmoType = AmmoType;

            // Add the ammo item to the inventory
            if (InventoryComponent->AddItem(AmmoItem))
            {
                Destroy();
            }
        }
    }
}
