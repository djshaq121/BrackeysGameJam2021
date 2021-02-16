// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerBase.generated.h"

class UInteractableComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class ETowerState : uint8 {
	Available,
	Building,
	Occupied
};

UCLASS()
class BRACKEYSGAMEJAM2021_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBase();

	UFUNCTION(BlueprintCallable, Category = "WaveGameMode")
	bool IsSpaceAvailable() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteract();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractableComponent* InteractableComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Tower State")
	ETowerState TowerState;
};
