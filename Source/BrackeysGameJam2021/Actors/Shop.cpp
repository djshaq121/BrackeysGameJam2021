// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"
#include "..\Assets\TowerData.h"
#include "../Public/WaveGameMode.h"
#include "Blueprint/UserWidget.h"
#include "../Actors/TowerBase.h"
#include "../Widgets/ShopUI.h"
#include "../Pawns/WavePlayer.h"
#include "GameFramework/PlayerController.h"

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
	
	ShopWidget = CreateWidget<UShopUI>(GetWorld(), ShopWidgetClass);
	ShopWidget->InitShop(this);
}

void AShop::InitShop(AWaveGameMode* gameMode, TArray<UTowerData*> towersData)
{
	WaveGameMode = gameMode;
	TowersData = towersData;
}

void AShop::OpenShop(ATowerBase* towerInstigator, AActor* playerInstigator)
{
	if (!towerInstigator)
	{
		// error
		UE_LOG(LogTemp, Error, TEXT("No Tower Instigator"));
		return;
	}
	
	PlayerInstigator = Cast<AWavePlayer>(playerInstigator);
	TowerBaseInstigator = towerInstigator;

	// Create widget and add to view port
	if (ShopWidget == nullptr)
	{
		ShopWidget = CreateWidget<UShopUI>(GetWorld(), ShopWidgetClass);
		ShopWidget->InitShop(this);
	}

	if (ShopWidget && !ShopWidget->IsVisible())
	{
		ShowPlayerCursor(true);
		if (PlayerInstigator)
		{
			PlayerInstigator->OnShowOpen();
			ShopWidget->WavePlayer = PlayerInstigator;
			auto pc = Cast<APlayerController>(PlayerInstigator->GetController());
			if (pc)
			{
				pc->SetInputMode(FInputModeGameAndUI());
			}
		}

		ShopWidget->UpdateTowerInstiagator(towerInstigator);
		ShopWidget->AddToViewport();
		TowerBaseInstigator->bIsShopOpen = true; 
	}
		
}

void AShop::CloseShop()
{
	ShowPlayerCursor(false);
	if (TowerBaseInstigator)
		TowerBaseInstigator->bIsShopOpen = false;

	TowerBaseInstigator = nullptr;
	ShopWidget->WavePlayer = nullptr;

	if (ShopWidget && ShopWidget->IsVisible())
	{
		if (PlayerInstigator)
		{
			PlayerInstigator->OnShopClose();
			auto pc = Cast<APlayerController>(PlayerInstigator->GetController());
			if (pc)
			{
				pc->SetInputMode(FInputModeGameOnly());
			}
		}
		ShopWidget->RemoveFromParent();
	}
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

void AShop::ShowPlayerCursor(bool bShowCursor)
{
	if (PlayerInstigator)
	{
		auto pc = Cast<APlayerController>(PlayerInstigator->GetController());
		if (pc)
		{
			pc->bShowMouseCursor = bShowCursor;
		}
	}
}