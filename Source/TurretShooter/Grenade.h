// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class TURRETSHOOTER_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrenade();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> GrenadeMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> ExplosionSystem;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> BounceSound;

	UPROPERTY(EditDefaultsOnly)
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly)
	float DamageAmount;

private:
	UFUNCTION()
	void onHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);

	void Explode();
	
	int bounceCount;
};
