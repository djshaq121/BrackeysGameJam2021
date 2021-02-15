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
struct FEnemyInfo
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
struct FWaveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	TArray<FEnemyInfo> EnemiesList;

};

USTRUCT(BlueprintType)
struct FLevelInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	TArray<FWaveInfo> WaveInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "1", UIMin = "1"), Category = "Wave")
	int32 TotalChances;
};

UCLASS()
class BRACKEYSGAMEJAM2021_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetNumberOfWaves() const;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetTotalAmountOfEnemies(int32 wave);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetTotalChances();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetEnemiesEscaped() const;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void EnemyEscaped();
	
protected:

	AWaveGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewEnemy(TSubclassOf<APawn> EnemyToSpawn, int32 RouteForEnemyToFollow);

	void BeginToSpawnEnemy();

	FWaveInfo* FetchWaveInfo(int32 wave);

	FLevelInfo* FetchLevelInfo(FName levelName);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 WaveRound = 0;

	FTimerHandle EnemySpawnerTimerHandle;

	FTimerHandle NextWaveTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeForNextWave = 2;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	UDataTable* WaveDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	FName CurrentLevelName = "Level1";

	int32 EnemySectionIndex = 0;

	int32 EnemiesSpawned = 0;

	FWaveInfo* CurrentWaveInfo;

	FLevelInfo* CurrentLevelInfo;

	int32 EnemiesEscaped = 0;

};
