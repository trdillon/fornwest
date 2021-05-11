// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FornwestGameMode.generated.h"

UCLASS(minimalapi)
class AFornwestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFornwestGameMode();

	virtual void BeginPlay() override;

	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Stats,
		HS_Skills,
		HS_Shop,
		HS_Crafting,
		HS_Pause
	};

	/** Changes the HUD based on the current HUDState. */
	void ChangeHUD();

	/** Gets the current HUDState */
	uint8 GetHUDState();

	/** Sets the current HUDState then calls ChangeHUD. */
	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void SetHUDState(uint8 NewState);

	/** Applies a HUD to the screen and returns whether or not it was successfully applied. */
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents);

protected:

	/** The current HUDState of the game. */
	uint8 HUDState;

	/** The current HUD widget being displayed. */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	/** The action bar HUD shown when the player is in the game. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> ActionBarHUDClass;

	/** The inventory HUD shown when the player is in the inventory window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> InventoryHUDClass;

	/** The stats HUD shown when the player is in the stats window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> StatsHUDClass;

	/** The skills HUD shown when the player is in the skills window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> SkillsHUDClass;

	/** The shop HUD shown when the player is in the shop window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> ShopHUDClass;

	/** The crafting HUD shown when the player is in the crafting window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> CraftingHUDClass;

	/** The pause HUD shown when the player is in the pause window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PauseHUDClass;
};