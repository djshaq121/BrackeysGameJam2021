// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class UCapsuleComponent;
UCLASS()
class BRACKEYSGAMEJAM2021_API APawnBase : public APawn
{
	GENERATED_BODY()

public:	
	// Sets default values for this pawn's properties
	APawnBase();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnLocation;

protected:
	void RotateTurret(FVector LookAtTarget);
	
	//You can add arguments to change the type of projectile fired and how many at a time. 
	void Fire();

	virtual void HandleDestruction();
};
