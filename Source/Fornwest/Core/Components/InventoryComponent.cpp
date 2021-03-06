// Fornwest ©2021 itsdits. All Rights Reserved.

#include "InventoryComponent.h"

#include "Fornwest/FornwestGameState.h"

UInventoryComponent::UInventoryComponent()
{
	SlotLimit = 24;
	WeightLimit = 50.0f;
	Inventory.SetNum(SlotLimit);
}

int32 UInventoryComponent::GetInventoryWeight()
{
	int32 Weight = 0;
	for (auto& Item : Inventory)
	{
		Weight += Item.Weight;
	}

	return Weight;
}

bool UInventoryComponent::AddItemToInventoryByID(const FName ID)
{
	//TODO - refactor this for better item access
	// Get the item from the ItemDB.
	AFornwestGameState* GameState = Cast<AFornwestGameState>(GetWorld()->GetGameState());
	UDataTable* ItemTable = GameState->GetItemDB();
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	// Check if we have an item then try to add it.
	if (ItemToAdd)
	{
		// Check if we have enough space and if the new item will put the inventory over the weight limit.
		if (Inventory.Num() < SlotLimit && (GetInventoryWeight() + ItemToAdd->Weight) <= WeightLimit)
		{
			// Add the item and broadcast the update event.
			Inventory.Add(*ItemToAdd);
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}
	
	return false;
}