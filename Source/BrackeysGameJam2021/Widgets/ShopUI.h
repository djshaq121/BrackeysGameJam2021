// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopUI.generated.h"

class AShop;
class ATowerBase;
class AWavePlayer;

UCLASS()
class BRACKEYSGAMEJAM2021_API UShopUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitShop(AShop* shopRef);

	void UpdateTowerInstiagator(ATowerBase* initiator);

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	AWavePlayer* WavePlayer;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	AShop* ShopOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	ATowerBase* TowerBaseInitiator;

};
