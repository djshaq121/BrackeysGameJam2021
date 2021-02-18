// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerBase.h"
#include "TimerManager.h"
#include "../Components/InteractableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Public/WaveGameMode.h"
#include "../Actors/Shop.h"
#include "../Assets/TowerData.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComponent = FindComponentByClass<UInteractableComponent>();
	if (!InteractableComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Interactable Component Found"));
		return;
	}

	InteractableComponent->OnInteract.AddDynamic(this, &ATowerBase::OnInteract);
	TowerState = ETowerState::Available;
	bIsShopOpen = false;
}

void ATowerBase::OnInteract(AActor* initiator)
{
	WaveGameMode = Cast<AWaveGameMode>(GetWorld()->GetAuthGameMode());
	if (!WaveGameMode)
		UE_LOG(LogTemp, Error, TEXT("Wave Game mode not found"));

	if (!Shop)
		Shop = WaveGameMode->Shop;

	if (Shop)
	{
		if (!bIsShopOpen)
		{
			Shop->OpenShop(this, initiator);
		}
		else
		{
			Shop->CloseShop();
		}
	}
}
void ATowerBase::BuildTower(UTowerData* towerToSpawn)
{
	TowerState = ETowerState::Building;
	BeginBuildingTower(towerToSpawn);
}

void ATowerBase::BeginBuildingTower(UTowerData* towerToSpawn)
{
	if (towerToSpawn && towerToSpawn->TowerActorClass)
	{
		
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName(TEXT("SpawnTower")), towerToSpawn->TowerActorClass);
		GetWorldTimerManager().SetTimer(BuildingTowerHandler, TimerDel, towerToSpawn->BuildingTime, false);
	}
}

void ATowerBase::SpawnTower(TSubclassOf<APawnBase> towerToSpawn)
{
	// If we have tower already destroy it 
	if (CurrentTower)
		CurrentTower->Destroy();
	
	if (towerToSpawn)
	{
		StaticMesh->GetSocketLocation(SpawnLocationSocketName);
		CurrentTower = GetWorld()->SpawnActor<APawnBase>(towerToSpawn, StaticMesh->GetSocketLocation(SpawnLocationSocketName), StaticMesh->GetSocketRotation(SpawnLocationSocketName));
		EndBuildingTower();
	}
}

bool ATowerBase::IsSpaceAvailable() const
{
	return TowerState == ETowerState::Available;
}

void ATowerBase::EndBuildingTower()
{
	TowerState = ETowerState::Occupied;
	GetWorldTimerManager().ClearTimer(BuildingTowerHandler);
}



