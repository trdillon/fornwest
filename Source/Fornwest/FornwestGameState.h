// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "FornwestGameState.generated.h"

/**
 * Implementation class for the game state.
 */
UCLASS()
class FORNWEST_API AFornwestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFornwestGameState();

	/** Returns the Item DB. */
	UDataTable* GetItemDB() const;

protected:
	/** The datatable containing all the items in the game. */
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDB;
};