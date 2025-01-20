// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthExhausted, FHitResult,Hit);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURRETSHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthExhausted OnHealthExhausted;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float DamageAmount ,FHitResult Hit = FHitResult());

	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthAmount);
	
private:
	float OriginalHealth;
};

