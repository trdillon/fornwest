// Fornwest ©2021 itsdits. All Rights Reserved.

#include "ManualPickup.h"

AManualPickup::AManualPickup()
{
	// Set the root component and default values.
	RootComponent = Cast<USceneComponent>(StaticMesh);
	ItemID = FName("No ID found");
	Name = TEXT("No name found");
	ActionText = TEXT("Pickup");
}

void AManualPickup::Interact_Implementation(ACharacter* Character)
{
	Super::Interact_Implementation(Character);

	// Get the inventory we want to add this to.
	if (Character)
	{
		//TODO - support multiple inventory components
		UActorComponent* Component = Character->GetComponentByClass(UInventoryComponent::StaticClass());
		UInventoryComponent* IInventory = Cast<UInventoryComponent>(Component);
		if (IInventory)
		{
			// Destroy the actor if it has been added to the inventory. We do this because the inventory will just hold
			// a struct with the item data, so the actor is no longer needed. We destroy it to free up memory instead of
			// just setting it invisible and disabling physics/collisions. If the player wants to drop the item into the
			// world we can spawn a new actor using the item data struct from the inventory.
			if (IInventory->AddItemToInventoryByID(ItemID))
			{
				Destroy();
			}
		}
	}
}