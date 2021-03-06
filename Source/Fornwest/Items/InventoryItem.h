// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

/**
 * Data class for items to be held in an inventory component. 
 */
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	FInventoryItem();

	/** The ID for this item in the ItemDB. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	/** The name of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	/** The description of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText Description;

	/** The weight of this item in kgs. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0))
	float Weight;

	/** If this item can be stacked with similar items or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStackable;

	/** The thumbnail for this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	//TODO - rarity, item category, stats and properties, value calc, durability/eth

	bool operator==(const FInventoryItem& OtherItem) const
	{
		if (ItemID == OtherItem.ItemID)
		{
			return true;
		}
			
		return false;
	}
};