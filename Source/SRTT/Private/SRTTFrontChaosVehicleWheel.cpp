// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTFrontChaosVehicleWheel.h"


USRTTFrontChaosVehicleWheel::USRTTFrontChaosVehicleWheel()
{
	// --- CORE CONFIGURATION ---
	// Front wheels should be affected by steering input.
	bAffectedBySteering = true;
	// Front wheels should be affected by brake input.
	bAffectedByBrake = true;
	// Front wheels are NOT powered by the engine in a rear-wheel-drive setup.
	bAffectedByEngine = false;
	// Front wheels are NOT affected by the handbrake.
	bAffectedByHandbrake = false;

	// --- PHYSICS DEFAULTS ---
	// These values are a good starting point and can be tuned in the Vehicle's Blueprint.
	AxleType = EAxleType::Front;
	WheelRadius = 35.f;
	WheelWidth = 20.f;
	MaxSteerAngle = 45.0f;
	MaxBrakeTorque = 3000.0f;
	MaxHandBrakeTorque = 0.0f; // No handbrake torque on front wheels
	WheelMass = 20.0f;
}
