// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Fornwest/Items/InventoryItem.h"
#include "InventoryComponent.generated.h"

// Blueprints can bind to this to update the UI.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

/**
 * Inventory component that holds items for an actor.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORNWEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	/** Gets the current weight of the inventory. */
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	int32 GetInventoryWeight();

	/** Adds an item to the inventory. */
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	bool AddItemToInventoryByID(FName ID);
	
	/** The inventory of items. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Inventory;

	/** The current amount of money in the inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Properties", meta = (ClampMin = 0, ClampMax = 999999999))
	int32 Money;

	/** The maximum amount of slots in the inventory. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Properties", meta = (ClampMin = 0, ClampMax = 24))
	int32 SlotLimit;

	/** The maximum weight the inventory will hold. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Properties", meta = (ClampMin = 0.0))
	float WeightLimit;

	/** Inventory change delegate. */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
};