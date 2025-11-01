// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTWheeledVehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

ASRTTWheeledVehiclePawn::ASRTTWheeledVehiclePawn()
{
	// Get the existing Chaos Vehicle Movement Component from the base class
	ChaosVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	ChaosVehicleMovementComponent->TransmissionSetup.bUseAutomaticGears = false;

	GearShiftVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GearShiftVFX"));
	GearShiftVFX->SetupAttachment(Mesh, TEXT("ExhaustSocket"));
	GearShiftVFX->bAutoActivate = false;

	// Create the Spring Arm Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bUsePawnControlRotation = true; // IMPORTANT: This makes the spring arm follow the controller's rotation
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance = 200.0f;

	// Create the Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // The camera should not rotate relative to the arm

	// --- INITIALIZE NEW VARIABLES ---
	ClutchInput = 0.0f;
	PreClutchGear = 1; // Default to 1st gear
	bIsClutchEngaged = false;
	NeutralEngineMOIMultiplier = 20.0f; // Default value, can be tweaked
	ClutchEngageThreshold = 0.9f;
}

void ASRTTWheeledVehiclePawn::ApplyThrottle_Implementation(float Value)
{
	if (ChaosVehicleMovementComponent)
	{
		// Set the throttle input on the movement component
		ChaosVehicleMovementComponent->SetThrottleInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::ApplySteer_Implementation(float Value)
{
	if (ChaosVehicleMovementComponent)
	{
		// Set the steering input on the movement component
		ChaosVehicleMovementComponent->SetSteeringInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::ApplyBrake_Implementation(float Value)
{
	if (ChaosVehicleMovementComponent)
	{
		// Set the brake input. A value of 0.0 means no brake.
		ChaosVehicleMovementComponent->SetBrakeInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::ApplyClutch_Implementation(float Value)
{
	ClutchInput = Value;
	const bool bIsClutchPressed = (ClutchInput >= ClutchEngageThreshold);

	if (bIsClutchPressed && !bIsClutchEngaged)
	{
		// --- CLUTCH WAS JUST PRESSED ---
		bIsClutchEngaged = true;

		// 1. Remember our current gear
		PreClutchGear = ChaosVehicleMovementComponent->GetCurrentGear();

		// 2. Shift to Neutral
		
		ChaosVehicleMovementComponent->SetTargetGear(0, true); // 0 is Neutral
		PlayGearShiftVFX();

		// 3. Make engine rev faster
		EngageNeutralEngineState();
	}
	else if (!bIsClutchPressed && bIsClutchEngaged)
	{
		// --- CLUTCH WAS JUST RELEASED ---
		bIsClutchEngaged = false;

		// 1. Shift back to the gear we were in
		ChaosVehicleMovementComponent->SetTargetGear(PreClutchGear, true);

		PlayGearShiftVFX();

		// 2. Restore engine to normal
		DisengageNeutralEngineState();
	}
}

// --- MODIFY EXISTING SHIFT FUNCTIONS ---

void ASRTTWheeledVehiclePawn::ShiftGearUp_Implementation()
{
	// --- FAILED SHIFT CHECK ---
	if (!bIsClutchEngaged) // Check if clutch is NOT pressed
	{
		// Get the controller (which implements the interface) and call the feedback function
		if (AController* MyController = GetController())
		{
			if (MyController->GetClass()->ImplementsInterface(UControllableVehicle::StaticClass()))
			{
				IControllableVehicle::Execute_TriggerFailedShiftEffect(MyController);
			}
		}
		return; // Stop here, do not change gear
	}
	
	// Do NOT change the gear here. Only update the "target" gear
	// that the clutch will engage when released.
	const int32 MaxGear = ChaosVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Num();

	// We update PreClutchGear, clamping it between -1 (Reverse) and MaxGear
	PreClutchGear = FMath::Clamp(PreClutchGear + 1, -1, MaxGear);
}

void ASRTTWheeledVehiclePawn::ShiftGearDown_Implementation()
{
	// --- FAILED SHIFT CHECK ---
	if (!bIsClutchEngaged) // Check if clutch is NOT pressed
	{
		if (AController* MyController = GetController())
		{
			if (MyController->GetClass()->ImplementsInterface(UControllableVehicle::StaticClass()))
			{
				IControllableVehicle::Execute_TriggerFailedShiftEffect(MyController);
			}
		}
		return; // Stop here, do not change gear
	}

	// Do NOT change the gear here. Only update the "target" gear
	const int32 MaxGear = ChaosVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Num();

	// We update PreClutchGear, clamping it between -1 (Reverse) and MaxGear
	PreClutchGear = FMath::Clamp(PreClutchGear - 1, -1, MaxGear);
}

// --- ADD NEW HELPER FUNCTION IMPLEMENTATIONS ---

void ASRTTWheeledVehiclePawn::EngageNeutralEngineState()
{
	// Multiply the values to make the engine more responsive
	ChaosVehicleMovementComponent->EngineSetup.EngineRevUpMOI *= NeutralEngineMOIMultiplier;
	ChaosVehicleMovementComponent->EngineSetup.EngineRevDownRate *= NeutralEngineMOIMultiplier;
}

void ASRTTWheeledVehiclePawn::DisengageNeutralEngineState()
{
	// Divide by the same multiplier to return to the default values
	// Add a check to prevent divide-by-zero
	ChaosVehicleMovementComponent->EngineSetup.EngineRevUpMOI /= NeutralEngineMOIMultiplier;
	ChaosVehicleMovementComponent->EngineSetup.EngineRevDownRate /= NeutralEngineMOIMultiplier;
}

// --- ADD EMPTY OVERRIDE FOR FEEDBACK ---
// This function is handled by the controller, but the pawn
// must have the override to satisfy the interface.
void ASRTTWheeledVehiclePawn::TriggerFailedShiftEffect_Implementation()
{
	// This is intentionally blank.
	// The pawn calls this, but the controller implements it.
}

void ASRTTWheeledVehiclePawn::SetHandbrake_Implementation(bool bIsEngaged)
{
	// Set the handbrake state
	ChaosVehicleMovementComponent->SetHandbrakeInput(bIsEngaged);
}

void ASRTTWheeledVehiclePawn::ApplyLook_Implementation(const FVector2D& LookAxisVector)
{
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

float ASRTTWheeledVehiclePawn::GetSpeedKPH_Implementation() const
{
	// GetForwardSpeed() returns speed in cm/s. 
	// (cm/s * 0.01) = m/s
	// (m/s * 3.6) = km/h
	return ChaosVehicleMovementComponent->GetForwardSpeed() * 0.036f;
}

float ASRTTWheeledVehiclePawn::GetEngineRPM_Implementation() const
{
	return ChaosVehicleMovementComponent->GetEngineRotationSpeed();
}

FString ASRTTWheeledVehiclePawn::GetGearAsString_Implementation() const
{
	if (ChaosVehicleMovementComponent)
	{
		const int32 CurrentGear = ChaosVehicleMovementComponent->GetCurrentGear();

		if (CurrentGear < 0)
		{
			return TEXT("R"); // Reverse
		}
		if (CurrentGear == 0)
		{
			return TEXT("N"); // Neutral
		}

		// For forward gears, convert the int to a string
		return FString::FromInt(CurrentGear);
	}
	return TEXT("N");
}

void ASRTTWheeledVehiclePawn::PlayGearShiftVFX()
{
	if (GearShiftVFX)
	{
		// Activate the particle system
		GearShiftVFX->Activate(true);

		// Clear any old timer that might be running
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_GearShiftVFX);

		// Set a new timer to deactivate the effect after 1.0 second
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_GearShiftVFX,
			this,
			&ASRTTWheeledVehiclePawn::DeactivateGearShiftVFX,
			1.0f,
			false
		);
	}
}

void ASRTTWheeledVehiclePawn::DeactivateGearShiftVFX()
{
	if (GearShiftVFX)
	{
		// Stop the particle system
		GearShiftVFX->Deactivate();
	}
}

