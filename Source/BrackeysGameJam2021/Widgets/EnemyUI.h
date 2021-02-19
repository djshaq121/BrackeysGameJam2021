// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUI.generated.h"

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM2021_API UEnemyUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetOwner(class AEnemy* newOwner) { enemyOwner = newOwner; }

	void SetHealthBarVisible(bool visible);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "EnemyUI")
	class AEnemy* enemyOwner;

	UPROPERTY(BlueprintReadOnly, Category = "EnemyUI")
	bool bHealthBarVisible = false;

	
};
