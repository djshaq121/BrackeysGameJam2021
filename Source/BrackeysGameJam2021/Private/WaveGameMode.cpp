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

	CurrentLevelInfo = FetchLevelInfo(CurrentLevelName);
	if (!CurrentLevelInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("Level Info Not found!"));
		return;
	}

	EnemiesEscaped = 0;
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
	UE_LOG(LogTemp, Error, TEXT("Begin Wave"));
	if (bGameIsOver)
		return;

	WaveRound++;
	
	CurrentWaveInfo = FetchWaveInfo(WaveRound);
	EnemySectionIndex = 0;
	EnemiesLeft = GetTotalAmountOfEnemies(WaveRound);

	GetWorldTimerManager().SetTimer(EnemySpawnerTimerHandle, this, &AWaveGameMode::BeginToSpawnEnemy, 1.0f, true, 0.0f);
}

void AWaveGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(EnemySpawnerTimerHandle);
	
	// For now just check if there is any more waves
	FWaveInfo* NextWaveInfo = FetchWaveInfo(WaveRound + 1);
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
		StopSpawningEnemies();
		//EndWave();
		return;
	}

	if (EnemiesSpawned < CurrentWaveInfo->EnemiesList[EnemySectionIndex].AmountToSpawn)
	{
		SpawnNewEnemy(CurrentWaveInfo->EnemiesList[EnemySectionIndex].EnemyToSpawn, CurrentWaveInfo->EnemiesList[EnemySectionIndex].RouteForEnemyToFollow);
		EnemiesSpawned++;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Next section"));
		EnemiesSpawned = 0;
		EnemySectionIndex++;
		BeginToSpawnEnemy();
	}
}

void AWaveGameMode::StopSpawningEnemies()
{
	GetWorldTimerManager().ClearTimer(EnemySpawnerTimerHandle);
}

FWaveInfo* AWaveGameMode::FetchWaveInfo(int32 wave)
{
	if (!WaveDataTable)
		return nullptr;

	if (!CurrentLevelInfo)
		return nullptr;

	if (CurrentLevelInfo->WaveInfo.Num() <= 0 || wave > CurrentLevelInfo->WaveInfo.Num())
		return nullptr;

	return &CurrentLevelInfo->WaveInfo[wave - 1];
}

FLevelInfo* AWaveGameMode::FetchLevelInfo(FName levelName)
{
	if (!WaveDataTable)
		return nullptr;

	static const FString contextString(TEXT(""));
	return WaveDataTable->FindRow<FLevelInfo>(CurrentLevelName, contextString, true);
}

int32 AWaveGameMode::GetNumberOfWaves() const
{
	if (!CurrentLevelInfo)
		return -1;

	return CurrentLevelInfo->WaveInfo.Num();
}

int32 AWaveGameMode::GetTotalAmountOfEnemies(int32 wave)
{
	if (!WaveDataTable)
		return 0;

	FWaveInfo* WaveInfo = FetchWaveInfo(wave);
	if (!WaveInfo)
		return -1;

	int32 total = 0;
	for (auto EnemySection : WaveInfo->EnemiesList)
	{
		total += EnemySection.AmountToSpawn;
	}

	return total;
}

int32 AWaveGameMode::GetTotalChances()
{
	if (!CurrentLevelInfo)
		return -1;

	return CurrentLevelInfo->TotalChances;
}

int32 AWaveGameMode::GetEnemiesEscaped() const
{
	return EnemiesEscaped;
}

void AWaveGameMode::EnemyEscaped()
{
	EnemiesEscaped++;

	if (GetTotalChances() < EnemiesEscaped)
	{
		// Game is over
		UE_LOG(LogTemp, Warning, TEXT("Game Over"));
		bGameIsOver = true;
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("One Esacped"));
}

void AWaveGameMode::UpdateEnemiesAlive()
{
	if (bGameIsOver)
		return;

	EnemiesLeft--;

	if (EnemiesLeft <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("All Enemies are dead - start next wave"));
		EndWave();
	}

}

