// Fornwest ©2021 itsdits. All Rights Reserved.

#include "StatsComponent.h"

UStatsComponent::UStatsComponent()
{
	CurrentLevel = 1;
	CurrentXP = 1;
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
	Strength = 20;
	Dexterity = 20;
	Fortitude = 20;
	Wisdom = 20;
}

void UStatsComponent::RegenerateHealth()
{
	CurrentHealth = FMath::Clamp(this->CurrentHealth += HealthRegenRate, 0.0f, MaxHealth);
}

void UStatsComponent::RegenerateMana()
{
	CurrentMana = FMath::Clamp(this->CurrentMana += ManaRegenRate, 0.0f, MaxMana);
}

void UStatsComponent::RegenerateStamina()
{
	CurrentStamina = FMath::Clamp(this->CurrentStamina += StaminaRegenRate, 0.0f, this->MaxStamina);
}

void UStatsComponent::DepleteStamina()
{
	CurrentStamina = FMath::Clamp(this->CurrentStamina -= StaminaDepleteRate, 0.0f, this->MaxStamina);
}