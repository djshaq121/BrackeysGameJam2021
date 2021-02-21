// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UHealthComponent;
class UAnimMontage;

UENUM(BlueprintType)
enum class EAbility : uint8 {
	SpeedBoost,
	HealthBoost,
	None
};

UCLASS()
class BRACKEYSGAMEJAM2021_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bIsdead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void ResetHealthBarVisbible();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float ProgressBarHideTime = 10.f;

	//This is the money the player will drop
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Money", meta = (AllowPrivateAccess = "true"))
	float Money = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Speed", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	//Initially set to none, but special enemies can have their own ability.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	EAbility ability = EAbility::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Comp")
	class UWidgetComponent* WidgetComp;

	UPROPERTY(BlueprintReadOnly, Category = "TowerBaseUI")
	class UEnemyUI* EnemyWidget;



	FTimerHandle LastTimeDamagerTimer;
};
