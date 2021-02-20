// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Components/HealthComponent.h"


AExplosiveProjectile::AExplosiveProjectile()
{
	ProjectileMovement->bIsHomingProjectile = false;
}
// Called when the game starts or when spawned
void AExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AExplosiveProjectile::OnHit);
}

void AExplosiveProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
		return;

	//If the other actor ISN't self OR Owner AND exists, then apply damage.
	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		EnemiesHit.Add(OtherActor);
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
	}
	
	ProjectileExplosion();
	Destroy();
}

void AExplosiveProjectile::ProjectileExplosion()
{
	TArray<FHitResult> HitResults;
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, 50.f);//Line trace a sphere around the player

	FCollisionShape CollionShape;
	CollionShape.ShapeType = ECollisionShape::Sphere;
	CollionShape.SetSphere(explosionRadius);

	FCollisionQueryParams CollParam;
	CollParam.AddIgnoredActor(this);//Ignore ourself
	CollParam.AddIgnoredActor(GetOwner());

	DrawDebugSphere(GetWorld(), End, explosionRadius, 12, FColor::Yellow, false, 3.f, 0, 1.f);

	if (GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::FQuat(), ECC_GameTraceChannel2, CollionShape, CollParam))
	{
		
		for (auto& Hit : HitResults)
		{
			AActor* actorHit = Hit.GetActor();
			if (!actorHit)
				continue;

			UHealthComponent* HealthComp = Cast<UHealthComponent>(Hit.GetActor()->GetComponentByClass(UHealthComponent::StaticClass()));
			
			if (HealthComp && !EnemiesHit.Contains(actorHit))
			{
				EnemiesHit.Add(actorHit);
				UGameplayStatics::ApplyDamage(actorHit, Damage * splashDamagePercent, GetOwner()->GetInstigatorController(), this, DamageType);
			}
		}
	}

}
