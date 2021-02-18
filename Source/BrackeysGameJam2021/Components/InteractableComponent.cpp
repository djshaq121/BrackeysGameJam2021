// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	InteractPromptText = "Interact";
}


void UInteractableComponent::Interact(AActor* instigator)
{
	OnInteract.Broadcast(instigator);
}