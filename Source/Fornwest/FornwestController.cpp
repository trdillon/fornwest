// Fornwest ©2021 itsdits. All Rights Reserved.

#include "FornwestController.h"
#include "FornwestGameMode.h"

AFornwestController::AFornwestController()
{
}

void AFornwestController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// A subclass could create a different InputComponent class but still want the default bindings
	if (InputComponent == nullptr)
	{
		InputComponent = NewObject<UInputComponent>(this, TEXT("PC_InputComponent0"));
		InputComponent->RegisterComponent();
	}

	check(InputComponent);
	
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AFornwestController::ToggleInventory);
}

void AFornwestController::ToggleInventory()
{
	// Check if inventory is already open, if so then close it.
	AFornwestGameMode* GameMode = Cast<AFornwestGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->GetHUDState() == GameMode->HS_Ingame)
	{
		GameMode->SetHUDState(GameMode->HS_Inventory);
	}
	else
	{
		GameMode->SetHUDState(GameMode->HS_Ingame);
	}
}