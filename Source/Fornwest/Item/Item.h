// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * Base class for all items in the game.
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class FORNWEST_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	virtual class UWorld* GetWorld() const override { return World; }

	/** The world the item is spawning in. */
	UPROPERTY(Transient)
	class UWorld* World;
	
	/** The unique ID of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FName ID;

	/** The name of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText Name;

	/** The description of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText Description;

	/** The interaction displayed when the item is focused on. IE: Use, Equip. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText Action;

	/** The weight of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float Weight;

	/** The sell value of the item. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	int32 Value;

	/** If the item can be stacked with similar items. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	bool IsStackable;

	/** The maximum size of an item stack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 1))
	int MaxStackSize;

	/** The static mesh to display when this item is visible for pickup. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* PickupMesh;

	/** The thumbnail of this item when viewed in the UI. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UTexture2D* Thumbnail;

	/** The inventory component that contains this item. */
	UPROPERTY()
	class UInventoryComponent* OwningInventory;

	/** Native Use function for the item. Pure virtual as it should be called on a child class. */
	virtual void Use(class AFornwestCharacter* Character) PURE_VIRTUAL(UItem, );

	/** Blueprint implementation of the Use function. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class AFornwestCharacter* Character);
};