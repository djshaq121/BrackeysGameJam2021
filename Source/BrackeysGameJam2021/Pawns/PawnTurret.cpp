// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Enemy.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	//EnemyCharacter = 
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EnemyCharacter || ReturnDistanceToPlayer() > FireRange) {
		return;
	}

	RotateTurret(EnemyCharacter->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
	if (!EnemyCharacter)
		return;

	if (ReturnDistanceToPlayer() <= FireRange) {
		//Fire
		Fire();
	}
}


float APawnTurret::ReturnDistanceToPlayer()
{
	if (!EnemyCharacter)
		return 0.0f;

	return FVector::Dist(EnemyCharacter->GetActorLocation(), GetActorLocation());
}


void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}