// Fornwest ©2021 itsdits. All Rights Reserved.

#include "Pickup.h"
#include "Fornwest/FornwestCharacter.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup()
{
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	InteractableMesh->SetSimulatePhysics(true);

	Name = TEXT("Name me please.");
	ActionText = TEXT("Press E to pick item up");
	Value = 0;
}

void APickup::BeginPlay()
{
	ActionText = FString::Printf(TEXT("%s: Press E to pick up."), *Name);
}

void APickup::Interact_Implementation()
{
	AFornwestCharacter* Character = Cast<AFornwestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Character->AddItemToInventory(this))
	{
		OnPickedUp();
	}

	//TODO - give user feedback about why it wasn't picked up
}

void APickup::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickup::Use_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup base class Use() called. You must call from the derived class."));
}