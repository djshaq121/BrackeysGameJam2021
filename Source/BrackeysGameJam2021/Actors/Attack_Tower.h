// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "Attack_Tower.generated.h"

class AProjectileBase;
class AEnemy;

UCLASS()
class BRACKEYSGAMEJAM2021_API AAttack_Tower : public ATower
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	class UHealthComponent* HealthComponent;

	TArray<AEnemy*> EnemyTargets;

	FTimerHandle FireRateTimerHandle;
	AEnemy* CurrentTarget;

private:
	void CheckFireCondition();

	float ReturnDistanceToPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTurret(FVector LookAtTarget);

	void StartFire();

	void EndFire();

	//You can add arguments to change the type of projectile fired and how many at a time. 
	void Fire();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	float LastFireTime;
};
