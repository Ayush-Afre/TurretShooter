// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	Health = 100.0f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalHealth = Health;
}

void UHealthComponent::ApplyDamage(float DamageAmount, FHitResult Hit)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Health = 0;
		OnHealthExhausted.Broadcast(Hit);
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
}

void UHealthComponent::AddHealth(float HealthAmount)
{
	if (Health + HealthAmount > OriginalHealth)
	{
		Health = OriginalHealth;
	}
	else
	{
		Health += HealthAmount;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
}
