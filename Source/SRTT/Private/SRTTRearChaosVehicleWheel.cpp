// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTRearChaosVehicleWheel.h"

USRTTRearChaosVehicleWheel::USRTTRearChaosVehicleWheel()
{
	// --- CORE CONFIGURATION ---
	// Rear wheels should NOT be affected by steering input.
	bAffectedBySteering = false;
	// Rear wheels should be affected by brake input.
	bAffectedByBrake = true;
	// Rear wheels ARE powered by the engine in a rear-wheel-drive setup.
	bAffectedByEngine = true;
	// Rear wheels ARE affected by the handbrake. This is critical for drifting.
	bAffectedByHandbrake = true;

	// --- PHYSICS DEFAULTS ---
	// These values are a good starting point and can be tuned in the Vehicle's Blueprint.
	AxleType = EAxleType::Rear;
	WheelRadius = 35.f;
	WheelWidth = 20.f;
	MaxSteerAngle = 0.0f; // No steering on rear wheels
	MaxBrakeTorque = 3000.0f;
	MaxHandBrakeTorque = 4000.0f;
	WheelMass = 20.0f;
}
