// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class AEnemy;

UCLASS()
class BRACKEYSGAMEJAM2021_API AProjectileBase : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetDamage(float damage) { Damage = damage; };
	void FollowTarget(AEnemy* Target);

	void Fire(AActor* Target);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed = 1300;

	float Damage = 50;

protected:

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
