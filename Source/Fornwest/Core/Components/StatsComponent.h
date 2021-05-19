// Fornwest ©2021 itsdits. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Fornwest/FornwestCharacter.h"

#include "StatsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORNWEST_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();

	/** Regenerates player health. */
	void RegenerateHealth();

	/** Regenerates player mana. */
	void RegenerateMana();

	/** Regenerates player stamina. */
	void RegenerateStamina();

	/** Depletes player stamina. */
	void DepleteStamina();
	
//protected:
	/** The current level of the character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	int32 CurrentLevel;

	/** The amount of experience the character currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CurrentXP;
	
	/** The max amount of health the character can have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;
	
	/** The max amount of mana the character can have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana;

	/** The max amount of stamina the character can have. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStamina;

	/** The amount of health the character currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentHealth;
	
	/** The amount of mana the character currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentMana;

	/** The amount of stamina the character currently has. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentStamina;

	/** The rate at which the character's health regenerates. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegenRate;

	/** The rate at which the character's mana regenerates. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRegenRate;

	/** The rate at which the character's stamina regenerates. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaRegenRate;

	/** The rate at which the character's stamina depletes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaDepleteRate;

	/** The amount of strength the character has. Strength increases melee attack power and total defense value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Strength;

	/** The amount of dexterity the character has. Dexterity increases ranged attack power and accuracy. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Dexterity;

	/** The amount of fortitude the character has. Fortitude increases maximum health and stamina. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Fortitude;

	/** The amount of wisdom the character has. Wisdom increases spell power and maximum mana. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Wisdom;

	//TODO - implement calculated stats like damage, armor, resistance, etc
};