// Fornwest ©2021 itsdits. All Rights Reserved.

#include "InventoryComponent.h"
#include "Item.h"

UInventoryComponent::UInventoryComponent()
{
	SlotLimit = 20;
	WeightLimit = 50.0f;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}

bool UInventoryComponent::AddItem(UItem* Item)
{
	// Break out if Item is null or player has full/overweight inventory.
	if (!Item || Items.Num() >= SlotLimit || GetInventoryWeight() + Item->Weight > WeightLimit)
	{
		return false;
	}
	
	Item->World = GetWorld();
	Item->OwningInventory = this;
	Items.Add(Item);

	// Update the UI.
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
	if (Item)
	{
		Item->World = nullptr;
		Item->OwningInventory = nullptr;
		Items.RemoveSingle(Item);

		// Update the UI.
		OnInventoryUpdated.Broadcast();
		
		return true;
	}

	return false;
}

float UInventoryComponent::GetInventoryWeight()
{
	float Weight = 0;
	for (auto& Item : Items)
	{
		Weight += Item->Weight;
	}

	return Weight;
}