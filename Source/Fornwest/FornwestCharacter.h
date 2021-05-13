// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "Core/Interactable.h"
#include "Core/Components/InventoryComponent.h"
#include "FornwestCharacter.generated.h"

// Blueprints can bind to these to update the UI.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnManaChanged);

UCLASS(config=Game)
class AFornwestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** Collection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

	/** The character's inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

public:
	AFornwestCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Heals the character. */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void Heal(float HealAmount);

	/** Damages the character. */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ApplyDamage(float DamageAmount);

	/** Uses an item. 
	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);
	*/
	/** Health change delegate. */
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnHealthChanged OnHealthChanged;

	/** Mana change delegate. */
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnManaChanged OnManaChanged;

	/** The action text that displays when the player focuses on an interactable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FText ActionText;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);
	
	/** Allows the character to begin sprinting. */
	void Sprint();

	/** Allows the character to stop sprinting. */
	void StopSprinting();

	/** Debug command to call TakeDamage. */
	void StartDamage();

	/** Debug command to use ability 1. */
	void UseAbility1();

	/** Handler for when a casting animation is finished. */
	void OnCastingFinish();

	/** Regenerates player health. */
	void RegenerateHealth();

	/** Regenerates player mana. */
	void RegenerateMana();

	/** Regenerates player stamina. */
	void RegenerateStamina();

	/** Depletes player stamina. */
	void DepleteStamina();
	
	/** The max amount of health the player can have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;
	
	/** The max amount of mana the player can have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana;

	/** The max amount of stamina the player can have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStamina;

	/** The amount of health the player currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentHealth;
	
	/** The amount of mana the player currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentMana;

	/** The amount of stamina the player currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentStamina;

	/** The rate at which the player's health regenerates. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegenRate;

	/** The rate at which the player's mana regenerates. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRegenRate;

	/** The rate at which the player's stamina regenerates. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaRegenRate;

	/** The rate at which the player's stamina depletes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaDepleteRate;

	/** Is the player currently casting a 1 handed spell or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool IsCasting1H;

	/** Is the player currently casting a 2 handed spell or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool IsCasting2H;

	/** Is the player currently casting a buff spell or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool IsCastingBuff;

	/** Is the player currently sprinting or not. */
	bool IsSprinting;

	/** Is the player currently in combat or not. */
	bool IsInCombat;

	/** Effect played on heal cast. */
	UPROPERTY(EditAnywhere, Category = "Ability")
	UParticleSystem* HealFX;

	/** Timer for waiting for the casting animation to finish. */
	FTimerHandle CastAnimationTimer;
	
	/** Timer calling health regeneration. */
	FTimerHandle HealthRegenTimer;
	
	/** Timer for calling mana regeneration. */
	FTimerHandle ManaRegenTimer;
	
	/** Timer for calling stamina regeneration. */
	FTimerHandle StaminaRegenTimer;

	/** Timer for calling stamina depletion. */
	FTimerHandle StaminaDepleteTimer;

private:

	/** Toggle the inventory window open and closed. */
	void ToggleInventory();

	/** Interact with an interactable object if there is one closed enough. */
	void Interact();
	
	/** Collects any auto pickups that the player comes in range of. */
	void CollectAutoPickups();
	
	/** Uses a line cast to check for interactables. Called on Tick. */
	void CheckForInteractables();

	/** The interactable the player is currently focused on. */
	AInteractable* CurrentInteractable;

protected:
	/** APawn interface */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom sub object. **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera sub object. **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
