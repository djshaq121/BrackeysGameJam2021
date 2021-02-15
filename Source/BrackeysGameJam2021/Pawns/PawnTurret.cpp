// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacter || ReturnDistanceToPlayer() > FireRange) {
		return;
	}

	RotateTurret(PlayerCharacter->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
	if (!PlayerCharacter)
		return;

	if (ReturnDistanceToPlayer() <= FireRange) {
		//Fire
		Fire();
	}
}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!PlayerCharacter)
		return 0.0f;

	return FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}