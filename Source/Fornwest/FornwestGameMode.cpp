// Copyright Epic Games, Inc. All Rights Reserved.

#include "FornwestGameMode.h"
#include "FornwestCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFornwestGameMode::AFornwestGameMode()
{
	// Set default pawn class to our Blueprinted character.
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Fornwest/Core/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Setup the action bar.
	HUDState = HS_Ingame;
}

void AFornwestGameMode::BeginPlay()
{
	// Apply the HUD on begin play.
	ChangeHUD();
}

void AFornwestGameMode::ChangeHUD()
{
	// Remove the previous HUD before adding the new one.
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	// Switch the HUD and input based on the current HUDState.
	switch (HUDState)
	{
		case HS_Ingame:
		{
			ApplyHUD(ActionBarHUDClass, false, false, false);
			break;
		}
		case HS_Inventory:
		{
			ApplyHUD(InventoryHUDClass, true, true, true);
			break;
		}
		case HS_Stats:
		{
			ApplyHUD(StatsHUDClass, true, true, true);
			break;
		}
		case HS_Skills:
		{
			ApplyHUD(SkillsHUDClass, true, true, true);
			break;
		}
		case HS_Shop:
		{
			ApplyHUD(ShopHUDClass, true, true, true);
			break;
		}
		case HS_Crafting:
		{
			ApplyHUD(CraftingHUDClass, true, true, true);
			break;
		}
		case HS_Pause:
		{
			ApplyHUD(PauseHUDClass, true, true, true);
			break;
		}
		default:
		{
			ApplyHUD(ActionBarHUDClass, false, false, false);
			break;
		}
	}
}

uint8 AFornwestGameMode::GetHUDState()
{
	return HUDState;
}

void AFornwestGameMode::SetHUDState(uint8 NewState)
{
	HUDState = NewState;
	ChangeHUD();
}

bool AFornwestGameMode::ApplyHUD(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor,
	bool bEnableClickEvents, bool bIsHUDClassUI)
{
	// Get a reference to the player and the controller.
	AFornwestCharacter* Character = Cast<AFornwestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	
	// Null check the widget we want to apply.
	if (WidgetToApply != nullptr)
	{
		// Create the widget.
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		// Set mouse cursor and click events.
		Controller->bShowMouseCursor = bShowMouseCursor;
		Controller->bEnableClickEvents = bEnableClickEvents;

		// Set the input mode.
		if (bIsHUDClassUI)
		{
			CurrentWidget->SetUserFocus(Controller);
			Controller->SetIgnoreLookInput(true);
			Controller->SetIgnoreMoveInput(true);
		}
		else
		{
			Controller->SetIgnoreLookInput(false);
			Controller->SetIgnoreMoveInput(false);
		}

		// If we have a widget then we add it to the viewport.
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			return true;
		}

		return false;
	}

	return false;
}