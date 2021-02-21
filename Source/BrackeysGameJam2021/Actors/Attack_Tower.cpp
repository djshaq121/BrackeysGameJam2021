// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack_Tower.h"
#include "ProjectileBase.h"
#include "BrackeysGameJam2021/Pawns/Enemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../Assets/TowerData.h"
#include "../Components/HealthComponent.h"


// Called when the game starts or when spawned
void AAttack_Tower::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent = FindComponentByClass<USphereComponent>();

	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAttack_Tower::OnOverlapBegin);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAttack_Tower::OnOverlapEnd);
		SphereComponent->SetSphereRadius(TowerRange);

	/*	TArray<AActor*> Result;
		SphereComponent->GetOverlappingActors(Result, AEnemy::StaticClass());
		for (auto actorFound : Result)
		{
			auto enemy = Cast<AEnemy>(actorFound);
			if (!enemy && EnemyTargets.Contains(enemy))
				return;

			EnemyTargets.Add(enemy);
			
		}*/
	}

	/*if (EnemyTargets.Num() > 0)
	{
		CurrentTarget = EnemyTargets[0];
	}
	else
	{
		CurrentTarget = nullptr;
	}*/
	
}

// Called every frame
void AAttack_Tower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentTarget || ReturnDistanceToPlayer() > TowerRange) {
		return;
	}

	RotateTurret(CurrentTarget->GetActorLocation());
}

void AAttack_Tower::CheckFireCondition()
{
	if (!CurrentTarget)
		return;

	Fire();
}

void AAttack_Tower::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

void AAttack_Tower::StartFire()
{
	if (!CurrentTarget)
		return;

	if (CurrentTarget->bIsdead)
	{
		if (EnemyTargets.Num() > 0)
		{
			CurrentTarget = EnemyTargets[0];
			return;
		}

	}
	float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->TimeSeconds, 0.0f);
	RotateTurret(CurrentTarget->GetActorLocation());

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AAttack_Tower::CheckFireCondition, FireRate, true, FirstDelay);
}

void AAttack_Tower::EndFire()
{
	UE_LOG(LogTemp, Warning, TEXT("EndFire"));
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
}

void AAttack_Tower::Fire()
{
	//Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class at location firing towards Rotation
	if (ProjectileClass) {
		FVector SpawnLocation = ProjectileSpawnLocation->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnLocation->GetComponentRotation();

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->FollowTarget(CurrentTarget);
		TempProjectile->SetOwner(this);
		TempProjectile->SetDamage(TowerDamge);
		LastFireTime = GetWorld()->TimeSeconds;
	}
}

float AAttack_Tower::ReturnDistanceToPlayer()
{
	if (!CurrentTarget)
		return 0.0f;

	return FVector::Dist(CurrentTarget->GetActorLocation(), GetActorLocation());
}

void AAttack_Tower::OnEnemyHealthChange(UHealthComponent* HealthComp, float Health, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
}

void AAttack_Tower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		auto enemy = Cast<AEnemy>(OtherActor);
		if (enemy) {
			if (!CurrentTarget) {
				CurrentTarget = enemy;
				StartFire();
			}
			EnemyTargets.Add(enemy);
		}
	}
}

void AAttack_Tower::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		auto enemy = Cast<AEnemy>(OtherActor);
		if (enemy) {
			if (enemy == CurrentTarget) { //Check to see if the enemy leaving the circle is the one we were targeting.
				EnemyTargets.RemoveSingle(enemy);
				if (EnemyTargets.Num() > 0) { //If we have anymore enemies in our list we can assign another one as the current target
					CurrentTarget = *(EnemyTargets.GetData());
				}
				else { //otherwise there are no enemies to be aimed at
					CurrentTarget = nullptr;
					EndFire();
				}
			}
			else {
				EnemyTargets.RemoveSingle(enemy);
			}
		}
	}
}

