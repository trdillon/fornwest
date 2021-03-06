// Fornwest ©2021 itsdits. All Rights Reserved.

#include "AutoPickup.h"

AAutoPickup::AAutoPickup()
{
	// Create the Static Mesh component and set the default values.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = Cast<USceneComponent>(StaticMesh);
	ItemID = FName("No ID found");
}

void AAutoPickup::Collect_Implementation(ACharacter* Character)
{
	// If we're calling this then we should make sure we are getting enough sleep at night..
	UE_LOG(LogTemp, Warning, TEXT("Collect() called from base class. You must call from the derived class."));
}

FName AAutoPickup::GetItemID()
{
	return ItemID;
}