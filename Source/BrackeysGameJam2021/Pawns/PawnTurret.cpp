// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "BrackeysGameJam2021/Actors/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "BrackeysGameJam2021/Components/HealthComponent.h"
#include "Enemy.h"
#include "../Assets/TowerData.h"

APawnTurret::APawnTurret() 
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APawnTurret::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APawnTurret::OnOverlapEnd);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));

	SphereComponent->SetSphereRadius(500.0f);
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComponent)
	{
		SphereComponent->SetSphereRadius(500.0f);
	}
	
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	CurrentTarget = nullptr; 
	
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentTarget || ReturnDistanceToPlayer() > FireRange) {
		return;
	}

	RotateTurret(CurrentTarget->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
	if (!CurrentTarget)
		return;

	if (ReturnDistanceToPlayer() <= FireRange) {
		//Fire
		Fire();
	}
}

void APawnTurret::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnTurret::Fire()
{
	//Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class at location firing towards Rotation
	if (ProjectileClass) {
		FVector SpawnLocation = ProjectileSpawnLocation->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnLocation->GetComponentRotation();

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!CurrentTarget)
		return 0.0f;

	return FVector::Dist(CurrentTarget->GetActorLocation(), GetActorLocation());
}

void APawnTurret::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void APawnTurret::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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


