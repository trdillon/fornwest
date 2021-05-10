// Fornwest ©2021 itsdits. All Rights Reserved.

#include "FoodItem.h"
#include "InventoryComponent.h"
#include "Fornwest/FornwestCharacter.h"

void UFoodItem::Use(AFornwestCharacter* Character)
{
	if (Character)
	{
		Character->Heal(HealthGain);

		if (OwningInventory)
		{
			OwningInventory->RemoveItem(this);
		}
	}
}