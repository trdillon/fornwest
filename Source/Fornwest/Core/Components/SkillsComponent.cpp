// Fornwest ©2021 itsdits. All Rights Reserved.

#include "SkillsComponent.h"

USkillsComponent::USkillsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}