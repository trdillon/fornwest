// Copyright Epic Games, Inc. All Rights Reserved.

#include "FornwestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/InventoryComponent.h"
#include "Item/Item.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AFornwestCharacter

AFornwestCharacter::AFornwestCharacter()
{
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
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character.
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller.

	// Create a follow camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation.
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm.

	// Create the inventory.
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->SlotLimit = 20;
	Inventory->WeightLimit = 50.0f;

	// Stats
	MaxHealth = 100.0f;
	MaxMana = 100.0f;
	MaxStamina = 100.0f;
	CurrentHealth = 100.0f;
	CurrentMana = 100.0f;
	CurrentStamina = 100.0f;
	HealthRegenRate = 0.25f;
	ManaRegenRate = 0.25f;
	StaminaRegenRate = 1.0f;
	StaminaDepleteRate = 1.0f;
	
	IsSprinting = false;
	IsCasting1H = false;
	IsCasting2H = false;
	IsCastingBuff = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFornwestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFornwestCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFornwestCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &AFornwestCharacter::StartDamage);

	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &AFornwestCharacter::UseAbility1);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFornwestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFornwestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFornwestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFornwestCharacter::LookUpAtRate);
}

void AFornwestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFornwestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFornwestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFornwestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFornwestCharacter::Sprint()
{
	IsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
	GetWorldTimerManager().SetTimer(StaminaDepleteTimer, this, &AFornwestCharacter::DepleteStamina, 0.05f, true);
}

void AFornwestCharacter::StopSprinting()
{
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetWorldTimerManager().SetTimer(StaminaRegenTimer, this, &AFornwestCharacter::RegenerateStamina, 0.1f, true, 1.00f);
}

void AFornwestCharacter::StartDamage()
{
	ApplyDamage(2.0f);
}

void AFornwestCharacter::Heal(float HealAmount)
{
	CurrentHealth += HealAmount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	OnHealthChanged.Broadcast();
}

void AFornwestCharacter::ApplyDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth < 0.00f)
	{
		CurrentHealth = 0.00f;
	}

	OnHealthChanged.Broadcast();
}

void AFornwestCharacter::UseItem(UItem* Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); // Blueprint version
	}
}

void AFornwestCharacter::UseAbility1()
{
	if (IsCasting1H || IsCasting2H || IsCastingBuff)
	{
		return;
	}

	if (CurrentMana < 15.0f)
	{
		return;
	}

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	CurrentMana -= 15.0f;
	OnManaChanged.Broadcast();
	Heal(15.0f);
	OnHealthChanged.Broadcast();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealFX, this->GetMesh()->GetSocketLocation("RightFoot"));
	
	IsCasting1H = true;

	// Wait for the casting to finish before moving on.
	GetWorldTimerManager().SetTimer(CastAnimationTimer, this, &AFornwestCharacter::OnCastingFinish, 2.00f, false);
	GetWorldTimerManager().SetTimer(HealthRegenTimer, this, &AFornwestCharacter::RegenerateHealth, 0.4f, true, 5.00f);
}

void AFornwestCharacter::OnCastingFinish()
{
	IsCasting1H = false;
	IsCasting2H = false;
	IsCastingBuff = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetWorldTimerManager().SetTimer(ManaRegenTimer, this, &AFornwestCharacter::RegenerateMana, 0.2f, true, 4.00f);
}

void AFornwestCharacter::RegenerateHealth()
{
	if (CurrentHealth == MaxHealth)
	{
		GetWorldTimerManager().ClearTimer(HealthRegenTimer);
		return;
	}
	
	if (!IsCasting1H && !IsCasting2H && !IsCastingBuff)
	{
		CurrentHealth = FMath::Clamp(this->CurrentHealth += HealthRegenRate, 0.0f, MaxHealth);
	}

	OnHealthChanged.Broadcast();
}

void AFornwestCharacter::RegenerateMana()
{
	if (CurrentMana == MaxMana)
	{
		GetWorldTimerManager().ClearTimer(ManaRegenTimer);
		return;
	}
	
	if (!IsCasting1H && !IsCasting2H && !IsCastingBuff)
	{
		CurrentMana = FMath::Clamp(this->CurrentMana += ManaRegenRate, 0.0f, MaxMana);
	}

	OnManaChanged.Broadcast();
}

void AFornwestCharacter::RegenerateStamina()
{
	if (CurrentStamina == MaxStamina)
	{
		GetWorldTimerManager().ClearTimer(StaminaRegenTimer);
		return;
	}
	
	if (this->CurrentStamina < MaxStamina && !IsSprinting)
	{
		CurrentStamina = FMath::Clamp(this->CurrentStamina += StaminaRegenRate, 0.0f, this->MaxStamina);
	}
}

void AFornwestCharacter::DepleteStamina()
{
	if (IsSprinting)
	{
		CurrentStamina = FMath::Clamp(this->CurrentStamina -= StaminaDepleteRate, 0.0f, this->MaxStamina);
		if (CurrentStamina <= 0)
		{
			GetWorldTimerManager().ClearTimer(StaminaDepleteTimer);
			StopSprinting();
		}
	}
}
