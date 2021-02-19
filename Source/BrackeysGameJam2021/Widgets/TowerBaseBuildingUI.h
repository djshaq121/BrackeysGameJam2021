// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Actors/TowerBase.h"
#include "TowerBaseBuildingUI.generated.h"


/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM2021_API UTowerBaseBuildingUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitTowerBaseUI(ATowerBase* initiator) { TowerBaseOwner = initiator; }

	void SetTimerHandlerBuilding(FTimerHandle buildingTowerHandler) { BuildingTowerHandler = buildingTowerHandler; }

	void SetBuildTime(float buildTime) { BuildTime = buildTime; }
protected:
	UPROPERTY(BlueprintReadOnly, Category = "TowerBaseUI")
	class ATowerBase* TowerBaseOwner;

	UPROPERTY(BlueprintReadOnly, Category = "TowerBaseUI")
	FTimerHandle BuildingTowerHandler;

	UPROPERTY(BlueprintReadOnly, Category = "TowerBaseUI")
	float BuildTime = 1.0f;
};
