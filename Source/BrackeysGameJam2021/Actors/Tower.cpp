// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BrackeysGameJam2021/Components/TowerComponent.h"
#include "../Assets/TowerData.h"

// Sets default values
ATower::ATower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);

	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Location"));
	ProjectileSpawnLocation->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	InitTowerData();
	TowerComponent = FindComponentByClass<UTowerComponent>();
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATower::InitTowerData()
{
	if (TowerData)
	{
		TowerRange = TowerData->Range;
		FireRate = TowerData->FiringRate;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Tower Data set"));
	}

}
