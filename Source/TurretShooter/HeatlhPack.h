// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "HeatlhPack.generated.h"

UCLASS()
class TURRETSHOOTER_API AHeatlhPack : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHeatlhPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* HealthMesh;

	UPROPERTY(EditDefaultsOnly)
	float HealthToAdd;

	void Interact_Implementation(APawn* InstigatorPawn);
};
