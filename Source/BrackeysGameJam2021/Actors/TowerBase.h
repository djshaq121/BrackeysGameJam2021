// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Pawns/PawnBase.h"
#include "TowerBase.generated.h"

class UInteractableComponent;
class UStaticMeshComponent;
class APawnBase;
class AWaveGameMode;
class AShop;
class UTowerData;
class UTowerBaseBuildingUI;

UENUM(BlueprintType)
enum class ETowerState : uint8 {
	Available,
	Building,
	Occupied
};

UCLASS()
class BRACKEYSGAMEJAM2021_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBase();

	UFUNCTION(BlueprintCallable, Category = "TowerBase")
	bool IsSpaceAvailable() const;

	UFUNCTION(BlueprintCallable, Category = "TowerBase")
	void BuildTower(UTowerData* towerToSpawn);

	UFUNCTION(BlueprintCallable, Category = "TowerBase")
	void SellTower(UTowerData* towerToSpawn);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool IsBuilding() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TowerBaseUI")
	TSubclassOf<class UTowerBaseBuildingUI> TowerBaseWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "TowerBaseUI")
	UTowerBaseBuildingUI* TowerBaseWidget;

	UFUNCTION()
	void OnInteract(AActor* initiator);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractableComponent* InteractableComponent;

	void BeginBuildingTower(UTowerData* towerToSpawn);

	UFUNCTION()
	void SpawnTower(UTowerData* towerToSpawnData);

	void EndBuildingTower();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Comp")
	class UWidgetComponent* WidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Static Mesh")
	FName SpawnLocationSocketName = "Base";

	UPROPERTY(BlueprintReadOnly, Category = "Tower State")
	ETowerState TowerState;

private:
	APawnBase* CurrentTower;

	AWaveGameMode* WaveGameMode;

	AShop* Shop;

	FTimerHandle BuildingTowerHandler;

public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTowerData* CurrentTowerData;
	// This allows us open and close the shop with the same interact button
	bool bIsShopOpen = false;

};
