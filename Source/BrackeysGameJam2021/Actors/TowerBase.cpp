// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerBase.h"
#include "../Components/InteractableComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComponent = FindComponentByClass<UInteractableComponent>();
	if (!InteractableComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Interactable Component Found"));
		return;
	}

	InteractableComponent->OnInteract.AddDynamic(this, &ATowerBase::OnInteract);
	TowerState = ETowerState::Available;
}

void ATowerBase::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("You interacted with tower base"));
}

bool ATowerBase::IsSpaceAvailable() const
{
	return TowerState == ETowerState::Available;
}
