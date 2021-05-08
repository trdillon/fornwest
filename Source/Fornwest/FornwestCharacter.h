// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FornwestCharacter.generated.h"

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

	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

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

	/** Heals the character. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	/** Damages the character. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float DamageAmount);

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

	/** Effect played on heal cast. */
	UPROPERTY(EditAnywhere, Category = "Ability")
	UParticleSystem* HealFX;

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
