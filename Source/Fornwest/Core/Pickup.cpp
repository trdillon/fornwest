// Fornwest ©2021 itsdits. All Rights Reserved.

#include "Pickup.h"
#include "Fornwest/FornwestCharacter.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup()
{
	// Set the physics so it acts like a real world item.
	StaticMesh->SetSimulatePhysics(true);

	// Set some default values.
	Name = FText::FromString("Name not found");
	Description = FText::FromString("Description not found");
	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultAsset(TEXT("/Game/Fornwest/Art/UI/T_BLANK"));
	Thumbnail = DefaultAsset.Object;
	Value = 0;
	IsStackable = false;
}

void APickup::BeginPlay()
{
	// Do we need this?
}

void APickup::Interact_Implementation()
{
	// Get the character.
	AFornwestCharacter* Character = Cast<AFornwestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Check if we can add the pickup to the character's inventory.
	if (Character->AddItemToInventory(this))
	{
		OnPickedUp();
	}

	// Failed to pick up. Better luck next time champ.
	//TODO - give user feedback about why it wasn't picked up
}

void APickup::OnPickedUp()
{
	// Remove it from the world to simulate picking the item up.
	StaticMesh->SetVisibility(false);
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickup::Use_Implementation()
{
	// If we're calling this then we need more coffee..
	UE_LOG(LogTemp, Warning, TEXT("Pickup base class Use() called. You must call from the derived class."));
}