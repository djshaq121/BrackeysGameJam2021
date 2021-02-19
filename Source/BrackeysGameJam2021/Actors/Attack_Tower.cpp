// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack_Tower.h"
#include "ProjectileBase.h"
#include "BrackeysGameJam2021/Pawns/Enemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../Assets/TowerData.h"


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
	}

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AAttack_Tower::CheckFireCondition, FireRate, true);
	CurrentTarget = nullptr;

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

	if (ReturnDistanceToPlayer() <= TowerRange) {
		//Fire
		Fire();
	}
}

void AAttack_Tower::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

void AAttack_Tower::Fire()
{
	//Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class at location firing towards Rotation
	if (ProjectileClass) {
		FVector SpawnLocation = ProjectileSpawnLocation->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnLocation->GetComponentRotation();

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

float AAttack_Tower::ReturnDistanceToPlayer()
{
	if (!CurrentTarget)
		return 0.0f;

	return FVector::Dist(CurrentTarget->GetActorLocation(), GetActorLocation());
}

void AAttack_Tower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On range"));
	if (OtherActor && (OtherActor != this) && OtherComp) {
		auto enemy = Cast<AEnemy>(OtherActor);
		if (enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Turret range"));
			if (!CurrentTarget) {
				CurrentTarget = enemy;
			}
			UE_LOG(LogTemp, Warning, TEXT("Added Character to TArray List"));
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
				}
			}
			else {
				EnemyTargets.RemoveSingle(enemy);
			}
		}
	}
}