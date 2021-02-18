// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrackeysGameJam2021/Pawns/PawnBase.h"
#include "PawnTurret.generated.h"

//Declare the enemy class. 
class AEnemy;
UCLASS()
class BRACKEYSGAMEJAM2021_API APawnTurret : public APawnBase
{
	GENERATED_BODY()
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	float FireRate = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	float FireRange = 500.0f;
	
	FTimerHandle FireRateTimerHandle;
	AEnemy* EnemyCharacter;

private:
	void CheckFireCondition();
	float ReturnDistanceToPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleDestruction() override;
};
