// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractableDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRACKEYSGAMEJAM2021_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void Interact();

	UPROPERTY(BlueprintAssignable)
	FInteractableDelegate OnInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactable")
	FString InteractPromptText = "";
		
};
