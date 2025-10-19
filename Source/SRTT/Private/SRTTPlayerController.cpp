// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTPlayerController.h"
#include "ControllableVehicle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

void ASRTTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// --- FIX: Assign the APawn* directly to the TScriptInterface ---
	// The TScriptInterface wrapper will automatically check if InPawn implements the interface.
	// If it doesn't, PossessedVehicle will be null, which is the correct behavior.
	PossessedVehicle = InPawn;
	// Add the Input Mapping Context
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (VehicleMappingContext)
			{
				Subsystem->AddMappingContext(VehicleMappingContext, 0);
			}
		}
	}

	// --- Create and display the HUD ---
	if (IsLocalController() && VehicleHUDClass)
	{
		// Only create it if it doesn't exist
		if (!VehicleHUDInstance)
		{
			VehicleHUDInstance = CreateWidget<UUserWidget>(this, VehicleHUDClass);
		}

		if (VehicleHUDInstance && !VehicleHUDInstance->IsInViewport())
		{
			VehicleHUDInstance->AddToViewport();
		}
	}
}

void ASRTTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Cast to the Enhanced Input Component
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		// Bind driving actions
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleThrottle);
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleSteer);
		EnhancedInputComponent->BindAction(GearUpAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleGearUp);
		EnhancedInputComponent->BindAction(GearDownAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleGearDown);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleBrake);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleThrottle);
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleSteer);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleBrake);

		// Handbrake requires separate events for pressed and released
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ASRTTPlayerController::HandleHandbrakePressed);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleHandbrakeReleased);

		// Bind camera look action
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleLook);
	}
}

void ASRTTPlayerController::HandleThrottle(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<float>();
	IControllableVehicle::Execute_ApplyThrottle(PossessedVehicle.GetObject(), Value);
}

void ASRTTPlayerController::HandleSteer(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<float>();
	IControllableVehicle::Execute_ApplySteer(PossessedVehicle.GetObject(), Value);
}

void ASRTTPlayerController::HandleGearUp(const FInputActionValue& ActionValue)
{
	IControllableVehicle::Execute_ShiftGearUp(PossessedVehicle.GetObject());
}

void ASRTTPlayerController::HandleGearDown(const FInputActionValue& ActionValue)
{
	IControllableVehicle::Execute_ShiftGearDown(PossessedVehicle.GetObject());
}

void ASRTTPlayerController::HandleBrake(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<float>();
	IControllableVehicle::Execute_ApplyBrake(PossessedVehicle.GetObject(), Value);
}

void ASRTTPlayerController::HandleHandbrakePressed(const FInputActionValue& ActionValue)
{
	IControllableVehicle::Execute_SetHandbrake(PossessedVehicle.GetObject(), true);
}

void ASRTTPlayerController::HandleHandbrakeReleased(const FInputActionValue& ActionValue)
{
	IControllableVehicle::Execute_SetHandbrake(PossessedVehicle.GetObject(), false);
}

void ASRTTPlayerController::HandleLook(const FInputActionValue& ActionValue)
{
	const FVector2D LookVector = ActionValue.Get<FVector2D>();
	IControllableVehicle::Execute_ApplyLook(PossessedVehicle.GetObject(), LookVector);
}

