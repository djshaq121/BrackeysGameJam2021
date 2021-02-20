// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "BrackeysGameJam2021/Components/HealthComponent.h"
#include "../Public/WaveGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "../Widgets/EnemyUI.h"
#include "TimerManager.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Comp"));
	WidgetComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChanged);

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if(CharacterMovementComponent)
		CharacterMovementComponent->MaxWalkSpeed = MovementSpeed;

	if (WidgetComp)
	{
		EnemyWidget = Cast<UEnemyUI>(WidgetComp->GetUserWidgetObject());
		if(EnemyWidget)
		{
			EnemyWidget->SetOwner(this);
			EnemyWidget->SetHealthBarVisible(false);
		}
		
	}
}

void AEnemy::OnHealthChanged(UHealthComponent* HealthComp, float Health, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsdead)
		return;

	if (Health <= 0.0f && !bIsdead) {
		
		bIsdead = true;

		if (EnemyWidget)
			EnemyWidget->SetHealthBarVisible(false);

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Call the GameMode to update currency, write a function for that in game mode and make sure that it is updating. 
		auto GameMode = Cast<AWaveGameMode>(GetWorld()->GetAuthGameMode());	
		if (GameMode)
		{
			GameMode->UpdatePlayerCurrency(Money);
		}
		//Enemy_BP class and call update enemies alive

		SetLifeSpan(3.0f);
		return;
	}

	if(EnemyWidget)
		EnemyWidget->SetHealthBarVisible(true);

	GetWorldTimerManager().ClearTimer(LastTimeDamagerTimer);
	GetWorldTimerManager().SetTimer(LastTimeDamagerTimer, this, &AEnemy::ResetHealthBarVisbible, ProgressBarHideTime, false);

}

void AEnemy::ResetHealthBarVisbible()
{
	EnemyWidget->SetHealthBarVisible(false);
	GetWorldTimerManager().ClearTimer(LastTimeDamagerTimer);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

