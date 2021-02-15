// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "WaveGameMode.generated.h"

/**
 * 
 */

class UDataTable;

USTRUCT(BlueprintType)
struct FEnemyInfoC
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category = "Wave")
	TSubclassOf<APawn> EnemyToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	int32 AmountToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	int32 RouteForEnemyToFollow;
};

USTRUCT(BlueprintType)
struct FWaveInfoTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	TArray<FEnemyInfoC> EnemiesList;
};

UCLASS()
class BRACKEYSGAMEJAM2021_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	AWaveGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewEnemy(TSubclassOf<APawn> EnemyToSpawn, int32 RouteForEnemyToFollow);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void GetCurrentWaveInfo(int32 CurrentWave);

	void BeginToSpawnEnemy();

	FWaveInfoTable* GetWaveInfo(int32 wave);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 WaveRound = 0;

	FTimerHandle EnemySpawnerTimerHandle;

	FTimerHandle NextWaveTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeForNextWave = 2;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	UDataTable* WaveDataTable;

	int32 EnemySectionIndex = 0;

	int32 EnemiesSpawned = 0;

	FWaveInfoTable* CurrentWaveInfo;


};
