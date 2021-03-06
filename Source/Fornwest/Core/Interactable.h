// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

/**
 * Base class for all interactable objects in the game.
 */
UCLASS()
class FORNWEST_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:
	AInteractable();

	/** Interact with this interactable. */
	UFUNCTION(BlueprintNativeEvent)
	void Interact(ACharacter* Character);
	virtual void Interact_Implementation(ACharacter* Character);

	/** Gets the action text to display when the interactable is focused on. */
	UFUNCTION(BlueprintCallable, Category = "Interactable Functions")
	FString GetActionText() const;
	
	/** The unique ID for this interactable. */
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
	FString Name;

	/** Action text to display for the player when the interactable is focused on. */
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
	FString ActionText;

	/** Static Mesh component for displaying the interactable in the world. */
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
	class UStaticMeshComponent* StaticMesh;
};