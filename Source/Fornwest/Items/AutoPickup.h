// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoPickup.generated.h"

/**
 * An actor that is automatically picked up when the player is close enough to it.
 */
UCLASS()
class FORNWEST_API AAutoPickup : public AActor
{
	GENERATED_BODY()
	
public:
	AAutoPickup();

	/** Collect this item. Called when the player's collection sphere collides with an auto pickup.*/
	UFUNCTION(BlueprintNativeEvent)
	void Collect(ACharacter* Character);
	virtual void Collect_Implementation(ACharacter* Character);

	/** Gets the ItemID of this pickup. */
	FName GetItemID();

protected:
	/** The ItemID for this item in the ItemDB. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	/** The StaticMesh component for this item. */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
};