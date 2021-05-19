// Copyright Epic Games, Inc. All Rights Reserved.

#include "FornwestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Interactable.h"
#include "Core/Components/InventoryComponent.h"
#include "Core/Components/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/AutoPickup.h"
#include "Kismet/GameplayStatics.h"

AFornwestCharacter::AFornwestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	// Set size for collision capsule.
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input.
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement.
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate.
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character.
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller.
	CameraBoom->SetupAttachment(RootComponent);
	
	// Create a follow camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm.
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation.

	// Create the auto collection sphere.
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	// Create the inventory.
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	// Create the stats component.
	Stats = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));
	
	IsSprinting = false;
	IsCasting1H = false;
	IsCasting2H = false;
	IsCastingBuff = false;
}

void AFornwestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFornwestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFornwestCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFornwestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFornwestCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFornwestCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFornwestCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFornwestCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFornwestCharacter::StopSprinting);
	PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &AFornwestCharacter::StartDamage);
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &AFornwestCharacter::UseAbility1);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFornwestCharacter::Interact);
}

void AFornwestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CollectAutoPickups();
	CheckForInteractables();
}

void AFornwestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find out which way is forward.
		const FRotator Rotation = this->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFornwestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// Find out which way is right.
		const FRotator Rotation = this->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// Get right vector.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// Add movement in that direction.
		AddMovementInput(Direction, Value);
	}
}

void AFornwestCharacter::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information.
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFornwestCharacter::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information.
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFornwestCharacter::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}

void AFornwestCharacter::CollectAutoPickups()
{
	// Get all overlapping AActors and store them in an array.
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// Loop the array and test if each can be picked up.
	for (int32 Collected = 0; Collected < CollectedActors.Num(); ++Collected)
	{
		// Cast the actor to AAutoPickup.
		AAutoPickup* const TestPickup = Cast<AAutoPickup>(CollectedActors[Collected]);
		
		// If the cast is successful and the pickup is valid and active then collect it.
		if (TestPickup && !TestPickup->IsPendingKill())
		{
			TestPickup->Collect(this);
		}
	}
}

void AFornwestCharacter::CheckForInteractables()
{
	// Get the start and end traces.
	int32 Range = 500;
	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = (FollowCamera->GetForwardVector() * Range) + StartTrace;

	// Ignore the player because we don't want a hit result on it.
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	// Cast the line trace.
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace,ECC_WorldDynamic, CollisionQueryParams))
	{
		// Attempt to cast to interactable if we have a hit result.
		AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());

		// If we found an interactable then set it as the current interactable.
		if (Interactable)
		{
			CurrentInteractable = Interactable;
			ActionText = FText::FromString(Interactable->GetActionText());
			return;
		}
	}

	ActionText = FText::FromString("");
	CurrentInteractable = nullptr;
}

void AFornwestCharacter::Sprint()
{
	IsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
	GetWorldTimerManager().SetTimer(StaminaDepleteTimer, this, &AFornwestCharacter::OnStaminaDeplete, 0.05f, true);
}

void AFornwestCharacter::StopSprinting()
{
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetWorldTimerManager().SetTimer(StaminaRegenTimer, this, &AFornwestCharacter::OnStaminaRegen, 0.1f, true, 1.00f);
}

void AFornwestCharacter::StartDamage()
{
	// Debug function to take damage.
	ApplyDamage(2.0f);
}

void AFornwestCharacter::Heal(const float HealAmount)
{
	Stats->CurrentHealth += HealAmount;
	if (Stats->CurrentHealth > Stats->MaxHealth)
	{
		Stats->CurrentHealth = Stats->MaxHealth;
	}
}

void AFornwestCharacter::ApplyDamage(const float DamageAmount)
{
	Stats->CurrentHealth -= DamageAmount;
	if (Stats->CurrentHealth < 0.00f)
	{
		Stats->CurrentHealth = 0.00f;
	}
}

void AFornwestCharacter::UseAbility1()
{
	if (IsCasting1H || IsCasting2H || IsCastingBuff)
	{
		return;
	}

	if (Stats->CurrentMana < 15.0f)
	{
		return;
	}

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	Stats->CurrentMana -= 15.0f;
	Heal(15.0f);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealFX, this->GetMesh()->GetSocketLocation("RightFoot"));
	
	IsCasting1H = true;

	// Wait for the casting to finish before moving on.
	GetWorldTimerManager().SetTimer(CastAnimationTimer, this, &AFornwestCharacter::OnCastingFinish, 2.00f, false);
	GetWorldTimerManager().SetTimer(HealthRegenTimer, this, &AFornwestCharacter::OnHealthRegen, 0.4f, true, 5.00f);
}

void AFornwestCharacter::OnHealthRegen()
{
	if (Stats->CurrentHealth == Stats->MaxHealth)
	{
		GetWorldTimerManager().ClearTimer(HealthRegenTimer);
		return;
	}

	if (!IsCasting1H && !IsCasting2H && !IsCastingBuff)
	{
		Stats->RegenerateHealth();
	}
}

void AFornwestCharacter::OnManaRegen()
{
	if (Stats->CurrentMana == Stats->MaxMana)
	{
		GetWorldTimerManager().ClearTimer(ManaRegenTimer);
		return;
	}
	
	if (!IsCasting1H && !IsCasting2H && !IsCastingBuff)
	{
		Stats->RegenerateMana();
	}
}

void AFornwestCharacter::OnStaminaRegen()
{
	if (Stats->CurrentStamina == Stats->MaxStamina)
	{
		GetWorldTimerManager().ClearTimer(StaminaRegenTimer);
		return;
	}
	
	if (Stats->CurrentStamina < Stats->MaxStamina && !IsSprinting)
	{
		Stats->RegenerateStamina();
	}
}

void AFornwestCharacter::OnStaminaDeplete()
{
	if (IsSprinting)
	{
		Stats->DepleteStamina();
		if (Stats->CurrentStamina <= 0)
		{
			GetWorldTimerManager().ClearTimer(StaminaDepleteTimer);
			StopSprinting();
		}
	}
}

void AFornwestCharacter::OnCastingFinish()
{
	IsCasting1H = false;
	IsCasting2H = false;
	IsCastingBuff = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetWorldTimerManager().SetTimer(ManaRegenTimer, this, &AFornwestCharacter::OnManaRegen, 0.2f, true, 4.00f);
}