// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnEnemyBase.generated.h"

class UCapsuleComponent;

/*
UENUM(BlueprintType)
enum class EAbility : uint8 {
	SpeedBoost,
	HealthBoost,
	None
};
*/

UCLASS()
class BRACKEYSGAMEJAM2021_API APawnEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnEnemyBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TakeDamage(float DamageAmount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BaseMesh;
	
	//This is the health we will need to set on the health component. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;
	
	//This is the money the player will drop
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Money", meta = (AllowPrivateAccess = "true"))
	float Money;
	
	//Initially set to none, but special enemies can have their own ability.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	//EAbility ability = EAbility::None;
};
