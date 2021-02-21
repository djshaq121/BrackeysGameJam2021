// Fill out your copyright notice in the Description page of Project Settings.


#include "WavePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Components/InteractableComponent.h"
#include "TimerManager.h"
#include "WaveGameMode.h"
#include "../Widgets/PlayerUI.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWavePlayer::AWavePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;//Setting the length of the cameraboom
	SpringArm->SocketOffset = FVector(0, 50, 50);//Setting the position of the camera
	SpringArm->bUsePawnControlRotation = true;//Stops the player rotating with the camera

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
} 

// Called when the game starts or when spawned
void AWavePlayer::BeginPlay()
{
	Super::BeginPlay();

	

	PlayerUI = CreateWidget<UPlayerUI>(GetWorld() ,PlayerUIClass);
	if (PlayerUI)
	{
		PlayerUI->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No player UI selected"));
	}
		

}

// Called every frame
void AWavePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHoldingDownStartWaveAction)
		HoldTime += DeltaTime;
		
	FindInteractableObject();
}

// Called to bind functionality to input
void AWavePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AWavePlayer::InteractPressed);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWavePlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWavePlayer::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("SkipPreparationPhase", IE_Pressed, this, &AWavePlayer::SkipPreparationPressed);
	PlayerInputComponent->BindAction("SkipPreparationPhase", IE_Released, this, &AWavePlayer::SkipPreparationReleased);

}

void AWavePlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWavePlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AWavePlayer::InteractPressed()
{
	if (InteractableCompHit)
	{
		InteractableCompHit->Interact(this);
	}
}

void AWavePlayer::FindInteractableObject()
{
	FHitResult Hit;
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	FVector End = CameraLoc + (CameraRot.Vector() * PlayerInteractionDistance);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, -1, 0, 5.f);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, CollisionParams))
	{
		auto ActorHit = Hit.GetActor();
		if (ActorHit)
		{
			auto interactableComp = ActorHit->FindComponentByClass<UInteractableComponent>();
			if (interactableComp)
			{
				//Show UI Promt
				if (PlayerUI)
					PlayerUI->UpdateInteractableText(interactableComp);
				
				InteractableCompHit = interactableComp;
				return;
			}
		}
	}

	InteractableCompHit = nullptr;
	if (PlayerUI)
		PlayerUI->UpdateInteractableText(nullptr);
}

void AWavePlayer::SkipPreparationPressed()
{
	HoldTime = 0;
	bIsHoldingDownStartWaveAction = true;
}

void AWavePlayer::SkipPreparationReleased()
{
	bIsHoldingDownStartWaveAction = false;
	if (HoldTime >= 0.3f)
	{
		AWaveGameMode* GameMode = Cast<AWaveGameMode>(GetWorld()->GetAuthGameMode());
		if (!GameMode)
			return;

		GameMode->SkipPreparationPhase();
	}
}


