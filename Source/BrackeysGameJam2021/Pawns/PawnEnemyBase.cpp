// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemyBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APawnEnemyBase::APawnEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APawnEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
