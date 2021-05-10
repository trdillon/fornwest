// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Blueprints can bind to this to update the UI.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

/**
 * Component for a character inventory that holds items.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORNWEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	virtual void BeginPlay() override;

	/** Adds item to the inventory if there is enough available space and weight. */
	bool AddItem(class UItem* Item);

	/** Removes item from the inventory. */
	bool RemoveItem(class UItem* Item);

	/** Gets the current weight of the inventory. */
	float GetInventoryWeight();

	/** Default items to start inventory with. */
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<class UItem*> DefaultItems;

	/** Items currently in the inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<class UItem*> Items;

	/** Item slot limit of the inventory. */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 SlotLimit;

	/** Weight limit of the inventory. */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float WeightLimit;

	/** Inventory change delegate. */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
};