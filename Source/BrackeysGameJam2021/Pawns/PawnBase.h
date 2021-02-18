// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"


class UCapsuleComponent;
class AProjectileBase;
class AEnemy;
class USphereComponent;

UCLASS()
class BRACKEYSGAMEJAM2021_API APawnBase : public APawn
{
	GENERATED_BODY()

public:	
	// Sets default values for this pawn's properties
	APawnBase();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitTowerData();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	class UTowerData* TowerData;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	float FireRate = 2.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"));
	float FireRange = 500.0f;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnLocation;
};
