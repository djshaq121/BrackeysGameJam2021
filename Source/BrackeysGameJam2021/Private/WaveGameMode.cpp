// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameMode.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
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

	// Create the widget
	if (HUDWaveSystemClass)
	{
		WaveSystemWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWaveSystemClass);
		if (WaveSystemWidget)
		{
			WaveSystemWidget->AddToViewport();
		}
	}

	WaveStatus = EWaveStatus::HasNotStarted;

	EnemiesEscaped = 0;
	PrepareForNextWave();
}

void AWaveGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AWaveGameMode::PrepareForNextWave()
{
	if (bGameIsOver)
		return;

	WaveStatus = EWaveStatus::Preparing; 

	WaveRound++;
	EnemiesLeft = GetTotalAmountOfEnemies(WaveRound);

	// We need to set the timer before the widget updates for NextWaveTimerHandle
	GetWorldTimerManager().SetTimer(NextWaveTimerHandle, this, &AWaveGameMode::StartWave, TimeForNextWave, false);

	//Update HUD elements
	UpdateOnPrepareForWave();
	UpdateWaveWidget(WaveRound, GetNumberOfWaves());
	UpdateEnemiesLeftWidget(EnemiesLeft);
	UpdateChancesLeftWidget(GetTotalChances() - EnemiesEscaped);
	//UpdateCurrencyWidget

}

void AWaveGameMode::StartWave()
{
	UE_LOG(LogTemp, Error, TEXT("Begin Wave"));
	if (bGameIsOver)
		return;

	WaveStatus = EWaveStatus::WaveActive;
	UpdateWidgetOnWaveStart();
	CurrentWaveInfo = FetchWaveInfo(WaveRound);
	EnemySectionIndex = 0;
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
		WaveStatus = EWaveStatus::LevelWon;
		bGameIsOver = true;
	}

}

void AWaveGameMode::BeginToSpawnEnemy()
{
	if (!CurrentWaveInfo)
		return;

	if (CurrentWaveInfo->EnemiesList.Num() == 0 || EnemySectionIndex > CurrentWaveInfo->EnemiesList.Num() - 1)
	{
		StopSpawningEnemies();
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

	UpdateChancesLeftWidget(FMath::Clamp(GetTotalChances() - EnemiesEscaped,0, INT_MAX));
	if (GetTotalChances() < EnemiesEscaped)
	{
		// Game is over
		WaveStatus = EWaveStatus::LevelLost;
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

	UpdateEnemiesLeftWidget(EnemiesLeft);
	if (EnemiesLeft <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("All Enemies are dead - End wave"));
		EndWave();
	}

}

// Only When in preparing Phase the user can start the wave straight away
void AWaveGameMode::StartWaveImmediately()
{
	if (bGameIsOver)
		return;

	if (WaveStatus == EWaveStatus::Preparing)
	{
		// Reset timer as it will start wave again
		GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);
		StartWave();
	}

}
