// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerBase.h"
#include "TimerManager.h"
#include "../Components/InteractableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Public/WaveGameMode.h"
#include "../Actors/Shop.h"
#include "../Assets/TowerData.h"
#include "../Widgets/TowerBaseBuildingUI.h"
#include "Components/WidgetComponent.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Comp"));
	WidgetComp->SetupAttachment(RootComponent);
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

	if (WidgetComp)
	{
		TowerBaseWidget = Cast<UTowerBaseBuildingUI>(WidgetComp->GetUserWidgetObject());
		//TowerBaseWidget = CreateWidget<UTowerBaseBuildingUI>(GetWorld(), TowerBaseWidgetClass);
		if (TowerBaseWidget)
		{
			//WidgetComp->SetWidgetClass(TowerBaseWidgetClass);
			TowerBaseWidget->InitTowerBaseUI(this);
			TowerBaseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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
	if (!towerToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No Tower Data"));
		return;
	}

	TowerState = ETowerState::Building;
	CurrentTowerData = towerToSpawn;
	BeginBuildingTower(towerToSpawn);
}

void ATowerBase::SellTower(UTowerData* towerToSpawn)
{
	// If we are building the tower we need to stop building
	GetWorldTimerManager().ClearTimer(BuildingTowerHandler);

	TowerState = ETowerState::Available;
	CurrentTowerData = nullptr;

	if (CurrentTower)
	{
		CurrentTower->Destroy();
		CurrentTower = nullptr;
	}
}

void ATowerBase::BeginBuildingTower(UTowerData* towerToSpawn)
{
	if (towerToSpawn && towerToSpawn->TowerActorClass)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName(TEXT("SpawnTower")), towerToSpawn);
		GetWorldTimerManager().SetTimer(BuildingTowerHandler, TimerDel, towerToSpawn->BuildingTime, false);
		if (TowerBaseWidget)
		{
			TowerBaseWidget->SetBuildTime((towerToSpawn->BuildingTime > 0) ? towerToSpawn->BuildingTime : 1);
			TowerBaseWidget->SetTimerHandlerBuilding(BuildingTowerHandler);
			TowerBaseWidget->SetVisibility(ESlateVisibility::Visible);

		}
	}
}

void ATowerBase::SpawnTower(UTowerData* towerToSpawnData)
{
	// If we have tower already destroy it 
	if (CurrentTower)
	{
		CurrentTowerData = nullptr;
		CurrentTower->Destroy();
	}
		
	
	if (towerToSpawnData && towerToSpawnData->TowerActorClass)
	{
		StaticMesh->GetSocketLocation(SpawnLocationSocketName);
		//CurrentTowerData = towerToSpawnData;
		CurrentTower = GetWorld()->SpawnActor<APawnBase>(towerToSpawnData->TowerActorClass, StaticMesh->GetSocketLocation(SpawnLocationSocketName), StaticMesh->GetSocketRotation(SpawnLocationSocketName));
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
	if (TowerBaseWidget)
		TowerBaseWidget->SetVisibility(ESlateVisibility::Hidden);
	
}

bool ATowerBase::IsBuilding() const
{
	return TowerState == ETowerState::Building;
}



