// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatlhPack.h"
#include "HealthComponent.h"

// Sets default values
AHeatlhPack::AHeatlhPack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthMesh"));
	RootComponent = HealthMesh;

	HealthToAdd = 10.0f;
}

// Called when the game starts or when spawned
void AHeatlhPack::BeginPlay()
{
	Super::BeginPlay();
}

void AHeatlhPack::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		TObjectPtr<UHealthComponent> HealthComponent = InstigatorPawn->FindComponentByClass<UHealthComponent>();
		if (HealthComponent)
		{
			HealthComponent->AddHealth(HealthToAdd);
		}
	}
	Destroy();
}
