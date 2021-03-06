// Fornwest ©2021 itsdits. All Rights Reserved.

#include "Interactable.h"

AInteractable::AInteractable()
{
	// Set some default values.
	Name = TEXT("Name not found");
	ActionText = TEXT("Press E to interact.");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

FString AInteractable::GetActionText() const
{
	return FString::Printf(TEXT("%s: Press E to %s"), *Name, *ActionText); 
}

void AInteractable::Interact_Implementation(ACharacter* Character)
{
	// If we're calling this then we did something very wrong..
	UE_LOG(LogTemp, Warning, TEXT("Interact base class Interact() called. You must call from the derived class."));
}