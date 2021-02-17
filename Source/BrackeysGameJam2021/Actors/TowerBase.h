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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteract();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractableComponent* InteractableComponent;

	void BeginBuildingTower(UTowerData* towerToSpawn);

	UFUNCTION()
	void SpawnTower(TSubclassOf<APawnBase> towerToSpawn);

	void EndBuildingTower();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Static Mesh")
	FName SpawnLocationSocketName = "Base";

	UPROPERTY(BlueprintReadOnly, Category = "Tower State")
	ETowerState TowerState;

private:
	APawnBase* CurrentTower;

	AWaveGameMode* WaveGameMode;

	AShop* Shop;

	FTimerHandle BuildingTowerHandler;

};
