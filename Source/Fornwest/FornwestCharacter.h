// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Interactable.h"
#include "Core/Pickup.h"
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

	/** Uses an item. */
	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);

	/** Health change delegate. */
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnHealthChanged OnHealthChanged;

	/** Mana change delegate. */
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnManaChanged OnManaChanged;

	/** The action text that displays when the player focuses on an interactable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString ActionText;

	/** The amount of money the player has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (ClampMin = 0))
	int32 Money;

	/** Updates the amount of money the player has.
	@param Amount This is the amount to update the money by. Can be positive or negative.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void UpdateMoney(int32 Amount);

	/** Adds an item to the player's inventory. */
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	bool AddItemToInventory(APickup* Item);

	/** Gets the name of the item at a given inventory slow. */
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	FString GetNameAtInventorySlot(int32 Slot);

	/** Gets the thumbnail for an item at a given inventory slot. */
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	/** Uses the item at a given inventory slot. */
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void UseAtInventorySlot(int32 Slot);

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
	
	///////// STATS //////////
	///
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
	
	///////// STATUS //////////
	///
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

	///////// FX //////////
	///
	/** Effect played on heal cast. */
	UPROPERTY(EditAnywhere, Category = "Ability")
	UParticleSystem* HealFX;

	///////// TIMERS //////////
	///
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
	
	/** Uses a line cast to check for interactables. Called on Tick. */
	void CheckForInteractables();

	/** The interactable the player is currently focused on. */
	AInteractable* CurrentInteractable;

	/** The player's inventory. */
	UPROPERTY(EditAnywhere)
	TArray<APickup*> Inventory;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
