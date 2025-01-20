// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class TURRETSHOOTER_API ATurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> TurretBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> TurretTop;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> DestroySound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCapsuleComponent> CapsulePlayerCheck;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> FireActor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> ExplosionSystem;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> TargetedSound;

	UPROPERTY(EditAnywhere)
	float TurnSpeed;

	UPROPERTY(EditAnywhere)
	float EndAngle;

	UPROPERTY(EditAnywhere)
	float ScanInterpSpeed;

	UPROPERTY(EditAnywhere)
	float LookAtInterpSpeed;

	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void TurretDestroyed();

private:
	UFUNCTION()
	void DestoryTurret(FHitResult Hit);

	UFUNCTION()
	void OnCheckBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                         const FHitResult& SweepResult);

	UFUNCTION()
	void OnCheckEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void CheckPlayerInView();

	void SetSmoothRotationTurretTop(FRotator NewRotation, float InterpSpeed);

	TObjectPtr<class ACharacter> PlayerCharacter;

	bool bIsTurretDestroyed;

	bool bIsPlayerOverlap;

	bool bIsPlayerTracked;

	float ScanRotation;

	FTimerHandle TimerHandle;
};
