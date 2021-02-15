// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameMode.h"
#include "TimerManager.h"
#include "Engine/DataTable.h"


AWaveGameMode::AWaveGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AWaveGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!WaveDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Wave Data table not found!"));
		return;
	}

	PrepareForNextWave();
}

void AWaveGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AWaveGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(NextWaveTimerHandle, this, &AWaveGameMode::StartWave, TimeForNextWave, false , 2.f);
}

void AWaveGameMode::StartWave()
{
	WaveRound++;
	
	CurrentWaveInfo = GetWaveInfo(WaveRound);
	EnemySectionIndex = 0;

	GetWorldTimerManager().SetTimer(EnemySpawnerTimerHandle, this, &AWaveGameMode::BeginToSpawnEnemy, 1.0f, true, 0.0f);
}

void AWaveGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(EnemySpawnerTimerHandle);
	

	// For now just check if there is any more waves
	FWaveInfoTable* NextWaveInfo = GetWaveInfo(WaveRound + 1);
	if (NextWaveInfo)
	{
		PrepareForNextWave();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("All Waves complete"));
	}

}

void AWaveGameMode::BeginToSpawnEnemy()
{
	if (!CurrentWaveInfo)
		return;

	if (CurrentWaveInfo->EnemiesList.Num() == 0 || EnemySectionIndex > CurrentWaveInfo->EnemiesList.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No More enemies for this Waves"));
		EndWave();
		return;
	}

	if (EnemiesSpawned < CurrentWaveInfo->EnemiesList[EnemySectionIndex].AmountToSpawn)
	{
		SpawnNewEnemy(CurrentWaveInfo->EnemiesList[EnemySectionIndex].EnemyToSpawn, CurrentWaveInfo->EnemiesList[EnemySectionIndex].RouteForEnemyToFollow);
		EnemiesSpawned++;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Next section"));
		EnemiesSpawned = 0;
		EnemySectionIndex++;
		BeginToSpawnEnemy();
	}
}

FWaveInfoTable* AWaveGameMode::GetWaveInfo(int32 wave)
{
	if (!WaveDataTable)
		return nullptr;

	static const FString contextString(TEXT(""));
	return WaveDataTable->FindRow<FWaveInfoTable>(FName(FString::FromInt(wave)), contextString, true);
}

int32 AWaveGameMode::GetNumberOfWaves() const
{
	if (!WaveDataTable)
		return -1;

	return WaveDataTable->GetRowNames().Num();
}

int32 AWaveGameMode::GetTotalAmountOfEnemies(int32 wave)
{
	if (!WaveDataTable)
		return 0;

	FWaveInfoTable* WaveInfo = GetWaveInfo(wave);
	if (!WaveInfo)
		return -1;

	int32 total = 0;
	for (auto EnemySection : WaveInfo->EnemiesList)
	{
		total += EnemySection.AmountToSpawn;
	}

	return total;
}



