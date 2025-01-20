// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	int MyInt = 10;
	FVector MyVector = FVector(10.0f,18.0f,34.0f);

	UE_LOG(LogTemp, Warning, TEXT("My int is %d"),MyInt);

	UE_LOG(LogTemp, Warning, TEXT("My Vector is %s"), *MyVector.ToString());
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

