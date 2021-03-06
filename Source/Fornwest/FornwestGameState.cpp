// Fornwest ©2021 itsdits. All Rights Reserved.

#include "FornwestGameState.h"

AFornwestGameState::AFornwestGameState()
{
	// Get the BP of the Item DB.
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("DataTable'/Game/Fornwest/Data/ItemDB.ItemDB'"));
	ItemDB = BP_ItemDB.Object;
}

UDataTable* AFornwestGameState::GetItemDB() const 
{
	return ItemDB; 
}