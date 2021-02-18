// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Components/InteractableComponent.h"
#include "PlayerUI.generated.h"

/**
 * 
 */

class UInteractableComponent;

UCLASS()
class BRACKEYSGAMEJAM2021_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerUI")
	void UpdateInteractableText(UInteractableComponent* InteractingActor);
};
