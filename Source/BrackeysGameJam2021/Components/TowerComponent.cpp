// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerComponent.h"

// Sets default values for this component's properties
UTowerComponent::UTowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTowerComponent::Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Using component"));
}

// Called when the game starts
void UTowerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

