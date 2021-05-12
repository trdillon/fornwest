// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

/**
 * Base class for an item that can be picked up in the world.
 */
UCLASS()
class FORNWEST_API APickup : public AInteractable
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	APickup();

	/** Called when play begins. */
	virtual void BeginPlay() override;

	/** Type of item this pickup is. */
	enum EItemType : uint8
	{
		IType_Consumable,
		IType_Armor,
		IType_Weapon,
		IType_Quest,
		IType_Special
	};

	/** Interact with this pickup. */
	virtual void Interact_Implementation() override;

	/** This is called when the item is picked up. */
	void OnPickedUp();

	/** Use this pickup. */
	UFUNCTION(BlueprintNativeEvent)
	void Use();
	virtual void Use_Implementation();

	/** The name of this pickup. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FText Name;

	/** The description of this pickup. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties", meta = (MultiLine = true))
	FText Description;

	/** The thumbnail this pickup to be displayed in the inventory and shop windows. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	UTexture2D* Thumbnail;

	/** The sell value of this pickup. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties", meta = (ClampMin = 0))
	int32 Value;

	/** If this pickup can be stacked with similar items or not. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	bool IsStackable;

	
};