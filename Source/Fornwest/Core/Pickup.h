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
	FString Name;

	/** The thumbnail this pickup to be displayed in the inventory and shop windows. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	UTexture2D* Thumbnail;

	/** The sell value of this pickup. */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties", meta = (ClampMin = 0))
	int32 Value;
};