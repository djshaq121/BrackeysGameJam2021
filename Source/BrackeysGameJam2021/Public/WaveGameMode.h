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
class UUserWidget;

UENUM(BlueprintType)
enum class EWaveStatus : uint8 {
	HasNotStarted,
	PreparingWave,
	WaveActive,
	LevelWon,
	LevelLost
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"), Category = "Wave")
	int32 StartingCurrency;
};

UCLASS()
class BRACKEYSGAMEJAM2021_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	void StartGame(FName levelName);

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	int32 GetNumberOfWaves() const;

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	int32 GetTotalAmountOfEnemies(int32 wave);

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	int32 GetTotalChances();

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	int32 GetEnemiesEscaped() const;

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	void EnemyEscaped();

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	void StopSpawningEnemies();

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	void UpdateEnemiesAlive();

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	int32 GetCurrency() const;

	void SkipPreparationPhase();

protected:

	AWaveGameMode();

	virtual void Tick(float DeltaSeconds) override;

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode")
	void SpawnNewEnemy(TSubclassOf<APawn> EnemyToSpawn, int32 RouteForEnemyToFollow);

	void BeginToSpawnEnemy();

	FWaveInfo* FetchWaveInfo(int32 wave);

	FLevelInfo* FetchLevelInfo(FName levelName);

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode - HUD")
	void UpdateWaveWidget(int32 wave, int32 totalWaves);

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode - HUD")
	void UpdateCurrencyWidget(int32 currency);

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode - HUD")
	void UpdateEnemiesLeftWidget(int32 enemiesLeft);

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode - HUD")
	void UpdateChancesLeftWidget(int32 Lives);

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode - HUD")
	void UpdateWidgetOnWaveStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "WaveGameMode - HUD")
	void UpdateOnPrepareForWave();

	void ResetGameMode();
public:

	UPROPERTY(BlueprintReadOnly, Category = Status)
	EWaveStatus WaveStatus;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WaveGameMode")
	int32 WaveRound = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WaveGameMode | Hud")
	FTimerHandle NextWaveTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "WaveGameMode")
	float TimeForNextWave = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "WaveGameMode")
	UDataTable* WaveDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "WaveGameMode")
	FName CurrentLevelName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveGameMode")
	TSubclassOf<class UUserWidget> HUDWaveSystemClass;

	UPROPERTY(BlueprintReadOnly, Category = "WaveGameMode | Hud")
	class UUserWidget* WaveSystemWidget;

	UPROPERTY(BlueprintReadOnly, Category = "WaveGameMode")
	int32 CurrentCurrency;

	FTimerHandle EnemySpawnerTimerHandle;

	int32 EnemySectionIndex = 0;

	int32 EnemiesSpawned = 0;

	// Enemies that have not died yet
	int32 EnemiesLeft = 0;

	FWaveInfo* CurrentWaveInfo;

	FLevelInfo* CurrentLevelInfo;

	int32 EnemiesEscaped = 0;

	bool bGameIsOver = false;

	
};
