// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FornwestController.generated.h"

/**
 * Controller class for a player character that handles input bindings and their functions.
 */
UCLASS()
class FORNWEST_API AFornwestController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	virtual void SetupInputComponent() override;

private:

	/** Toggle the inventory window open and closed. */
	void ToggleInventory();
};