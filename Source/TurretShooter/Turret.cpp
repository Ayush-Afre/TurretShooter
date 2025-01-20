// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "BulletProjectile.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "HealthComponent.h"

// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	RootComponent = TurretBase;
	TurretBase->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	TurretTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretTop"));
	TurretTop->SetupAttachment(TurretBase);
	TurretTop->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	CapsulePlayerCheck = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsulePlayerCheck"));
	CapsulePlayerCheck->SetupAttachment(TurretTop);
	CapsulePlayerCheck->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CapsulePlayerCheck->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnCheckBeginOverlap);
	CapsulePlayerCheck->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnCheckEndOverlap);
	CapsulePlayerCheck->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(TurretTop);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	ScanRotation = 0.0f;
	TurnSpeed = 10.0f;
	EndAngle = 60.0f;
	LookAtInterpSpeed = 12.0f;
	ScanInterpSpeed = 5.0f;
	FireRate = 0.5f;

	bIsTurretDestroyed = false;
	bIsPlayerOverlap = false;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnHealthExhausted.AddDynamic(this, &ATurret::DestoryTurret);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ScanRotation += TurnSpeed * DeltaTime;

	if (FMath::Abs(ScanRotation) > EndAngle)
	{
		TurnSpeed = TurnSpeed * -1;
	}

	if (PlayerCharacter && (bIsPlayerOverlap || bIsPlayerTracked))
	{
		CheckPlayerInView();

		if (bIsPlayerTracked)
		{
			FRotator LookAtRotWorld = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(),
			                                                                 TurretTop->GetComponentLocation());
			FRotator LookAtRotLocal = UKismetMathLibrary::InverseTransformRotation(GetActorTransform(), LookAtRotWorld);
			SetSmoothRotationTurretTop(LookAtRotLocal, LookAtInterpSpeed);
			if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATurret::Fire, FireRate, true);
			}
		}
		else
		{
			SetSmoothRotationTurretTop(FRotator(0.0f, ScanRotation, 0.0f), ScanInterpSpeed);
		}
	}
	else
	{
		SetSmoothRotationTurretTop(FRotator(0.0f, ScanRotation, 0.0f), ScanInterpSpeed);
	}


	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Scan Rotation: %f"), ScanRotation));
}

void ATurret::DestoryTurret(FHitResult Hit)
{
	if (!bIsTurretDestroyed)
	{
		TurretTop->SetSimulatePhysics(true);
		PrimaryActorTick.SetTickFunctionEnable(false);

		if (Hit.ImpactPoint != FVector(0, 0, 0))
		{
			TurretTop->AddImpulseAtLocation((Hit.ImpactNormal + FVector(0.0f, 0.0f, 0.1f)) * 10000, Hit.ImpactPoint);
		}
		else
		{
			TurretTop->AddImpulseAtLocation(
				(FVector(FMath::FRandRange(-0.5f, 0.5f), FMath::FRandRange(-0.5f, 0.5f), 1.0f)) * 10000,
				GetActorLocation());
		}

		UGameplayStatics::PlaySoundAtLocation(this, DestroySound, GetActorLocation(), 0.5f);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionSystem,
													   GetActorLocation() + FVector(0.0f, 0.0f, 10.0f), GetActorRotation());
		bIsPlayerTracked = false;
		bIsTurretDestroyed = true;

		TurretDestroyed();
	}
}

void ATurret::OnCheckBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	TObjectPtr<ACharacter> Character = Cast<ACharacter>(OtherActor);

	if (Character)
	{
		PlayerCharacter = Character;
		bIsPlayerOverlap = true;
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
		//                                  FString::Printf(TEXT("Begin Overlap Actor: %s"), *OtherActor->GetName()));
	}
}

void ATurret::OnCheckEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<ACharacter> Character = Cast<ACharacter>(OtherActor);

	if (Character)
	{
		bIsPlayerOverlap = false;
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
		//                                  FString::Printf(TEXT("End Overlap Actor: %s"), *OtherActor->GetName()));
	}
}

void ATurret::Fire()
{
	if (bIsPlayerTracked)
	{
		if (FireActor)
		{
			GetWorld()->SpawnActor<AActor>(FireActor, Arrow->GetComponentTransform());
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.5f);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlash, Arrow->GetComponentLocation(),
			                                               Arrow->GetComponentRotation());
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void ATurret::CheckPlayerInView()
{
	FHitResult HitResult;
	FVector StartLocation = TurretTop->GetComponentLocation() + FVector(0.0f, 0.0f, 50.0f);
	FVector EndLocation = PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, PlayerCharacter->BaseEyeHeight);

	FCollisionObjectQueryParams ObjectTypeParams;
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectTypeParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectTypeParams,
	                                                    CollisionParams);
	// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);

	if (bHit)
	{
		if (HitResult.GetActor() == PlayerCharacter)
		{
			if (!bIsPlayerTracked)
			{
				UGameplayStatics::PlaySoundAtLocation(this, TargetedSound, GetActorLocation(), 1.0f);
			}
			bIsPlayerTracked = true;

			return;
		}
	}
	bIsPlayerTracked = false;
}

void ATurret::SetSmoothRotationTurretTop(FRotator NewRotation, float InterpSpeed)
{
	TurretTop->SetRelativeRotation(
		UKismetMathLibrary::RInterpTo(TurretTop->GetRelativeRotation(), NewRotation, GetWorld()->GetDeltaSeconds(),
		                              InterpSpeed));
}
