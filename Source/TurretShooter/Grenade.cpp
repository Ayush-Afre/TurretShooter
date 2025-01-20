// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "HealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMesh");
	GrenadeMesh->SetSimulatePhysics(true);
	RootComponent = GrenadeMesh;
	GrenadeMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	GrenadeMesh->SetNotifyRigidBodyCollision(true);
	GrenadeMesh->OnComponentHit.AddDynamic(this, &AGrenade::onHit);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->Velocity = FVector(2000.0f, 0.0f, 0.0f);

	DamageRadius = 160.0f;
	DamageAmount = 20.0f;
}

void AGrenade::onHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                     FVector NormalImpulse, const FHitResult& Hit)
{
	bounceCount++;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BounceSound, GetActorLocation(), 0.5f);

	if (bounceCount > 5 || OtherActor->IsA(ACharacter::StaticClass()))
	{
		Explode();
	}
}

void AGrenade::Explode()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(), 0);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionSystem, GetActorLocation(),
	                                               GetActorRotation());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

	TArray<AActor*> OverlappedActors;

	bool bOverlapped = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), DamageRadius, ObjectTypes,
		nullptr, TArray<TObjectPtr<AActor>>(), OverlappedActors);

	// DrawDebugSphere(
	// 	GetWorld(), GetActorLocation(), DamageRadius, 16, FColor::Red,
	// 	false, 10.0f, 0, 2.0f);

	if (bOverlapped)
	{
		for (AActor* OverlappedActor : OverlappedActors)
		{
			TObjectPtr<UHealthComponent> HealthComp = OverlappedActor->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				HealthComp->ApplyDamage(DamageAmount);
			}
		}
	}


	Destroy();
}
