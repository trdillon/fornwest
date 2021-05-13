// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "FoodItem.generated.h"

/**
 * Consumable item that restores the player's health when eaten.
 */
UCLASS()
class FORNWEST_API UFoodItem : public UItem
{
	GENERATED_BODY()

public:
	/** The amount of health the player regains from eating this food. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float HealthGain;
	
protected:
	virtual void Use(AFornwestCharacter* Character) override;
};