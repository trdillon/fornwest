// Copyright Epic Games, Inc. All Rights Reserved.

#include "FornwestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AFornwestCharacter

AFornwestCharacter::AFornwestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PlayerHealth = 1.00f;
	PlayerStamina = 1.00f;
	IsSprinting = false;
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

	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AFornwestCharacter::StartHealing);
	PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &AFornwestCharacter::StartDamage);

	PlayerInputComponent->BindAction("Ability 1", IE_Pressed, this, &AFornwestCharacter::UseAbility1);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFornwestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFornwestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFornwestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFornwestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFornwestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFornwestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFornwestCharacter::OnResetVR);
}

void AFornwestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/** Regenerate stamina if not running, or drain it if running. */
	if (this->PlayerStamina < 1.00f && !IsSprinting)
	{
		this->PlayerStamina += 0.1f * DeltaTime;
		if (PlayerStamina > 1.00f)
		{
			PlayerStamina = 1.00f;
		}
	}
	if (IsSprinting)
	{
		this->PlayerStamina -= 0.4f * DeltaTime;
		if (PlayerStamina <= 0)
		{
			PlayerStamina = 0;
			StopSprinting();
		}
	}
}


void AFornwestCharacter::OnResetVR()
{
	// If Fornwest is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Fornwest.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFornwestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AFornwestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
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
}

void AFornwestCharacter::StopSprinting()
{
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AFornwestCharacter::StartHealing()
{
	Heal(0.02f);
}

void AFornwestCharacter::StartDamage()
{
	ApplyDamage(0.02f);
}

void AFornwestCharacter::Heal(float HealAmount)
{
	PlayerHealth += HealAmount;

	if (PlayerHealth > 1.00f)
	{
		PlayerHealth = 1.00f;
	}
}

void AFornwestCharacter::ApplyDamage(float DamageAmount)
{
	PlayerHealth -= DamageAmount;

	if (PlayerHealth < 0.00f)
	{
		PlayerHealth = 0.00f;
	}
}

void AFornwestCharacter::UseAbility1()
{
	FTimerHandle Timer;
	IsCasting1H = true;
	GetWorldTimerManager().SetTimer(Timer, this, &AFornwestCharacter::OnCastingFinish, 2.00f, false);
}

void AFornwestCharacter::OnCastingFinish()
{
	IsCasting1H = false;
	IsCasting2H = false;
	IsCastingBuff = false;
}
