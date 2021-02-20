// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ExplosiveProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM2021_API AExplosiveProjectile : public AProjectileBase
{
	GENERATED_BODY()

	AExplosiveProjectile();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ProjectileExplosion();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float explosionRadius = 400.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1", ClampMax = "1", UIMin = "0.1", UIMax = "1"), Category = "Projectile")
	float splashDamagePercent = 0.3f;

	TArray<AActor*> EnemiesHit;

	
};
