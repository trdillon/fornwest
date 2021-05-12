// Fornwest ©2021 itsdits. All Rights Reserved.

#include "Interactable.h"

AInteractable::AInteractable()
{
	// Set some default values.
	ID = TEXT("ID not found");
	ActionText = FText::FromString("Press E to interact.");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

void AInteractable::BeginPlay()
{
	// We will need this implemented on some interactables.
	Super::BeginPlay();
}

void AInteractable::Interact_Implementation()
{
	// If we're calling this then we did something very wrong..
	UE_LOG(LogTemp, Warning, TEXT("Interact base class Interact() called. You must call from the derived class."));
}