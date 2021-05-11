// Fornwest ©2021 itsdits. All Rights Reserved.

#include "Interactable.h"

AInteractable::AInteractable()
{
	ActionText = TEXT("Press E to interact with this.");
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractable::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact base class Interact() called. You must call from the derived class."));
}