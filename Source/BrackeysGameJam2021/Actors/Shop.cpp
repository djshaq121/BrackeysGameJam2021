// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"
#include "..\Assets\TowerData.h"
#include "../Public/WaveGameMode.h"
#include "Blueprint/UserWidget.h"
#include "../Actors/TowerBase.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	
	ShopWidget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
}

void AShop::InitShop(AWaveGameMode* gameMode, TArray<UTowerData*> towersData)
{
	WaveGameMode = gameMode;
	TowersData = towersData;
}

void AShop::OpenShop(ATowerBase* instigator)
{
	TowerBaseInstigator = instigator;

	// Create widget and add to view port
	if (ShopWidget == nullptr)
	{
		ShopWidget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
	}

	if (ShopWidget && !ShopWidget->IsVisible())
		ShopWidget->AddToViewport();
}

bool AShop::BuyTower(UTowerData* TowerToBuy)
{
	
	if (!TowerToBuy || !CanAffordTower(TowerToBuy))
		return false;

	if (!TowerBaseInstigator)
		return false;

	if (!TowerBaseInstigator->IsSpaceAvailable())
		return false;

	// Update Currency
	WaveGameMode->UpdatePlayerCurrencyFromShop(TowerToBuy->Price);

	TowerBaseInstigator->BuildTower(TowerToBuy);

	return true;
}

bool AShop::CanAffordTower(UTowerData* TowerToBuy)
{
	if (!WaveGameMode)
		return false;

	return WaveGameMode->GetCurrency() >= TowerToBuy->Price;
}

