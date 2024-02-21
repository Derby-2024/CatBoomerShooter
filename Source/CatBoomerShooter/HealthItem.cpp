// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItem.h"
#include "InventoryComponent.h"
#include "Character/BasePlayerCharacter.h"

UHealthItem::UHealthItem()
{
	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void UHealthItem::Use(ABasePlayerCharacter* Character)
{
	if (Character)
	{
		Character->Health += HealthToHeal;

		if (OwningInventory)
		{
			OwningInventory->RemoveItem(this);
		}
	}
}
