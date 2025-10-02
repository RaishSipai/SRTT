// Copyright SRTT Studios. All Rights Reserved.

#include "SRTTPlayerController.h"
#include "SRTTWheeledVehiclePawn.h"
#include "SRTTGearboxComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

void ASRTTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASRTTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// --- BIND DRIVING ACTIONS ---
		if (ThrottleAction)
		{
			EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleThrottle);
			EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleThrottleReleased);
		}
		if (SteerAction)
		{
			EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleSteer);
			EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleSteerReleased);
		}
		if (BrakeAction)
		{
			EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleBrake);
			EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleBrakeReleased);
		}
		if (HandbrakeAction)
		{
			EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleHandbrake);
			EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleHandbrakeReleased);
		}

		// --- BIND MANUAL TRANSMISSION ACTIONS ---
		if (ClutchAction)
		{
			EnhancedInputComponent->BindAction(ClutchAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleClutch);
			EnhancedInputComponent->BindAction(ClutchAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleClutch);
		}
		if (GearUpAction)
		{
			EnhancedInputComponent->BindAction(GearUpAction, ETriggerEvent::Started, this, &ASRTTPlayerController::HandleGearUp);
		}
		if (GearDownAction)
		{
			EnhancedInputComponent->BindAction(GearDownAction, ETriggerEvent::Started, this, &ASRTTPlayerController::HandleGearDown);
		}
	}
}

void ASRTTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(InPawn))
	{
		if (USRTTGearboxComponent* Gearbox = VehiclePawn->FindComponentByClass<USRTTGearboxComponent>())
		{
			Gearbox->OnShiftFailed.AddDynamic(this, &ASRTTPlayerController::OnShiftFailed);
		}
	}
}

// --- Triggered Input Handlers ---

void ASRTTPlayerController::HandleThrottle(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetThrottleInput(Value.Get<float>());
	}
}

void ASRTTPlayerController::HandleSteer(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetSteeringInput(Value.Get<float>());
	}
}

void ASRTTPlayerController::HandleBrake(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetBrakeInput(Value.Get<float>());
	}
}

void ASRTTPlayerController::HandleHandbrake(const FInputActionValue& Value)
{
	// Corrected the typo here
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetHandbrakeInput(Value.Get<float>());
	}
}

// --- Completed (Released) Input Handlers ---

void ASRTTPlayerController::HandleThrottleReleased(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetThrottleInput(0.0f);
	}
}

void ASRTTPlayerController::HandleSteerReleased(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetSteeringInput(0.0f);
	}
}

void ASRTTPlayerController::HandleBrakeReleased(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetBrakeInput(0.0f);
	}
}

void ASRTTPlayerController::HandleHandbrakeReleased(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetHandbrakeInput(0.0f);
	}
}


// --- Manual Transmission Handlers ---

void ASRTTPlayerController::HandleClutch(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->HandleClutchInput(Value.Get<float>());
	}
}

void ASRTTPlayerController::HandleGearUp(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->HandleShiftUp();
	}
}

void ASRTTPlayerController::HandleGearDown(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->HandleShiftDown();
	}
}

// --- Feedback Handler ---

void ASRTTPlayerController::OnShiftFailed()
{
	if (ShiftFailCameraShake)
	{
		ClientStartCameraShake(ShiftFailCameraShake);
	}

	if (ShiftFailForceFeedback)
	{
		ClientPlayForceFeedback(ShiftFailForceFeedback);
	}
}

