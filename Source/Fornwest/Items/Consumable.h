// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Consumable.generated.h"

/**
 * Consumable items are one time use items for the player to eat, drink, craft, etc.
 */
UCLASS()
class FORNWEST_API AConsumable : public APickup
{
	GENERATED_BODY()

public:
	AConsumable();

	/** Type of item this pickup is. */
	enum EConsumableType : uint8
	{
		CType_Food,
		CType_Drink,
		CType_Potion,
		CType_Resource,
		CType_Gem,
		CType_Ingredient
	};

	/** The maximum stack size of this pickup item if it is stackable. */
    UPROPERTY(EditAnywhere, Category = "Pickup Properties", meta = (ClampMin = 0))
    int32 MaxStackSize;
};