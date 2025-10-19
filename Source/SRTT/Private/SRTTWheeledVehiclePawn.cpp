// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTWheeledVehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"


ASRTTWheeledVehiclePawn::ASRTTWheeledVehiclePawn()
{
	// Get the existing Chaos Vehicle Movement Component from the base class
	ChaosVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	ChaosVehicleMovementComponent->TransmissionSetup.bUseAutomaticGears = false;

	// Create the Spring Arm Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Mesh, TEXT("Root"));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bUsePawnControlRotation = true; // IMPORTANT: This makes the spring arm follow the controller's rotation

	// Create the Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // The camera should not rotate relative to the arm
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

void ASRTTWheeledVehiclePawn::ShiftGearUp_Implementation()
{
	if (ChaosVehicleMovementComponent)
	{
		const int32 CurrentGear = ChaosVehicleMovementComponent->GetCurrentGear();

		// --- !!! THIS IS THE CORRECTED LINE FOR MAX GEAR !!! ---
		// We get the number of forward gears from the TransmissionSetup's array
		const int32 MaxGear = ChaosVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Num();

		// Calculate the target gear, clamping it between -1 (Reverse) and MaxGear
		const int32 TargetGear = FMath::Clamp(CurrentGear + 1, -1, MaxGear);

		ChaosVehicleMovementComponent->SetTargetGear(TargetGear, true);
	}
}

void ASRTTWheeledVehiclePawn::ShiftGearDown_Implementation()
{
	if (ChaosVehicleMovementComponent)
	{
		const int32 CurrentGear = ChaosVehicleMovementComponent->GetCurrentGear();

		// --- !!! THIS IS THE CORRECTED LINE FOR MAX GEAR !!! ---
		const int32 MaxGear = ChaosVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Num();

		// Calculate the target gear, clamping it between -1 (Reverse) and MaxGear
		const int32 TargetGear = FMath::Clamp(CurrentGear - 1, -1, MaxGear);

		ChaosVehicleMovementComponent->SetTargetGear(TargetGear, true);
	}
}

void ASRTTWheeledVehiclePawn::SetHandbrake_Implementation(bool bIsEngaged)
{
	if (ChaosVehicleMovementComponent)
	{
		// Set the handbrake state
		ChaosVehicleMovementComponent->SetHandbrakeInput(bIsEngaged);
	}
}

void ASRTTWheeledVehiclePawn::ApplyLook_Implementation(const FVector2D& LookAxisVector)
{
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

float ASRTTWheeledVehiclePawn::GetSpeedKPH_Implementation() const
{
	if (ChaosVehicleMovementComponent)
	{
		// GetForwardSpeed() returns speed in cm/s. 
		// (cm/s * 0.01) = m/s
		// (m/s * 3.6) = km/h
		return ChaosVehicleMovementComponent->GetForwardSpeed() * 0.036f;
	}
	return 0.0f;
}

float ASRTTWheeledVehiclePawn::GetEngineRPM_Implementation() const
{
	if (ChaosVehicleMovementComponent)
	{
		return ChaosVehicleMovementComponent->GetEngineRotationSpeed();
	}
	return 0.0f;
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

