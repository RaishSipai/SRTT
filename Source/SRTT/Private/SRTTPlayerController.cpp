// Copyright SRTT Studios. All Rights Reserved.

#include "SRTTPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SRTTWheeledVehiclePawn.h"
#include "SRTTGearboxComponent.h"
#include "SRTTWeaponSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "GameFramework/Pawn.h"

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
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleThrottle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleThrottleCompleted);

		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleSteer);
		EnhancedInputComponent->BindAction(SteerAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleSteerCompleted);

		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleBrakeCompleted);

		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleHandbrakeCompleted);

		EnhancedInputComponent->BindAction(ClutchAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleClutch);
		EnhancedInputComponent->BindAction(ClutchAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleClutchCompleted);

		// These are single-press actions, so 'Started' is correct
		EnhancedInputComponent->BindAction(GearUpAction, ETriggerEvent::Started, this, &ASRTTPlayerController::HandleGearUp);
		EnhancedInputComponent->BindAction(GearDownAction, ETriggerEvent::Started, this, &ASRTTPlayerController::HandleGearDown);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::HandleLook);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASRTTPlayerController::HandleFireStarted);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::HandleFireCompleted);
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

// --- Manual Transmission Handlers ---

void ASRTTPlayerController::HandleClutch(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		if (USRTTGearboxComponent* Gearbox = VehiclePawn->FindComponentByClass<USRTTGearboxComponent>())
		{
			Gearbox->HandleClutchInput(Value.Get<float>());
		}
	}
}

void ASRTTPlayerController::HandleGearUp(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		if (USRTTGearboxComponent* Gearbox = VehiclePawn->FindComponentByClass<USRTTGearboxComponent>())
		{
			Gearbox->HandleShiftUp();
		}
	}
}

void ASRTTPlayerController::HandleGearDown(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		if (USRTTGearboxComponent* Gearbox = VehiclePawn->FindComponentByClass<USRTTGearboxComponent>())
		{
			Gearbox->HandleShiftDown();
		}
	}
}

void ASRTTPlayerController::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (LookAxisVector.X != 0.f)
	{
		AddYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddPitchInput(LookAxisVector.Y);
	}
}

void ASRTTPlayerController::HandleFireStarted()
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		if (USRTTWeaponSystemComponent* WeaponSystem = VehiclePawn->FindComponentByClass<USRTTWeaponSystemComponent>())
		{
			WeaponSystem->StartFire();
		}
	}
}

void ASRTTPlayerController::HandleFireCompleted()
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		if (USRTTWeaponSystemComponent* WeaponSystem = VehiclePawn->FindComponentByClass<USRTTWeaponSystemComponent>())
		{
			WeaponSystem->StopFire();
		}
	}
}

void ASRTTPlayerController::HandleThrottleCompleted(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetThrottleInput(0.0f);
	}
}

void ASRTTPlayerController::HandleSteerCompleted(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetSteeringInput(0.0f);
	}
}

void ASRTTPlayerController::HandleBrakeCompleted(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetBrakeInput(0.0f);
	}
}

void ASRTTPlayerController::HandleHandbrakeCompleted(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		VehiclePawn->SetHandbrakeInput(0.0f);
	}
}

void ASRTTPlayerController::HandleClutchCompleted(const FInputActionValue& Value)
{
	if (ASRTTWheeledVehiclePawn* VehiclePawn = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
	{
		if (USRTTGearboxComponent* Gearbox = VehiclePawn->FindComponentByClass<USRTTGearboxComponent>())
		{
			Gearbox->HandleClutchInput(0.0f);
		}
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
		FForceFeedbackParameters FFParams;
		FFParams.bLooping = false;
		ClientPlayForceFeedback(ShiftFailForceFeedback, FFParams);
	}
}

