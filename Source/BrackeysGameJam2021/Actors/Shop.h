// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

class UTowerData;
class ATowerBase;
class AWaveGameMode;

UCLASS()
class BRACKEYSGAMEJAM2021_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveGameMode")
	TSubclassOf<class UUserWidget> ShopWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	class UUserWidget* ShopWidget;

public:	
	void InitShop(AWaveGameMode* gameMode, TArray<UTowerData*> towersData);

	void OpenShop(ATowerBase* instigator);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool BuyTower(UTowerData* TowerToBuy);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool CanAffordTower(UTowerData* TowerToBuy);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TArray<UTowerData*> TowersData;

	AWaveGameMode* WaveGameMode;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	ATowerBase* TowerBaseInstigator;
};
