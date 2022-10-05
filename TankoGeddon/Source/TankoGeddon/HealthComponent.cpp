// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float takeDamageValue = DamageData.DamageValue;
	CurrentHealth -= takeDamageValue; // CurrentHealth = CurrentHealth - takeDamageValue;

	if (CurrentHealth <= 0)
	{
		if(OnDie.IsBound())
			OnDie.Broadcast();
	}
	else
	{
		if(OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(takeDamageValue);
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

void UHealthComponent::AddHealth(float newHealth)
{
	CurrentHealth += newHealth;
	if (CurrentHealth >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

float UHealthComponent::GetHealthState()
{
	return CurrentHealth/MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}


