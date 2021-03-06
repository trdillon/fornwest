// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fornwest/FornwestCharacter.h"
#include "Fornwest/Core/Interactable.h"
#include "ManualPickup.generated.h"

/**
 * An interactable that requires a player action to pickup.
 */
UCLASS()
class FORNWEST_API AManualPickup : public AInteractable
{
	GENERATED_BODY()

public:
	AManualPickup();

	virtual void Interact_Implementation(ACharacter* Character) override;

protected:
	/** The ItemID for this item in the ItemDB. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
};