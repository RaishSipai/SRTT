// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTTFrontChaosVehicleWheel.h"
#include "UObject/ConstructorHelpers.h"

USRTTFrontChaosVehicleWheel::USRTTFrontChaosVehicleWheel()
{
    // Front wheel configuration for steering and braking
    
    // Enable steering for front wheels
    bAffectedBySteering = true;
    
    // Enable braking for front wheels
    bAffectedByBrake = true;
    
    // Disable engine drive for front wheels (rear-wheel drive)
    bAffectedByEngine = false;
    
    // Disable handbrake for front wheels
    bAffectedByHandbrake = false;
    
    // Wheel physics settings
    WheelRadius = 35.0f;
    WheelWidth = 20.0f;
    
    // Suspension settings
    SuspensionForceOffset = FVector::ZeroVector;
    SuspensionMaxRaise = 10.0f;
    SuspensionMaxDrop = 10.0f;
    SuspensionDampingRatio = 1.2f;
    
    // Tire friction settings
    FrictionForceMultiplier = 2.0f;
    
    // Steering settings
    MaxSteerAngle = 45.0f;
    
    // Brake settings
    MaxBrakeTorque = 1500.0f;
    
    // Wheel mass and inertia
    WheelMass = 20.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Front wheel configured - Steering: %s, Brake: %s, Engine: %s, Handbrake: %s"), 
           bAffectedBySteering ? TEXT("Enabled") : TEXT("Disabled"),
           bAffectedByBrake ? TEXT("Enabled") : TEXT("Disabled"),
           bAffectedByEngine ? TEXT("Enabled") : TEXT("Disabled"),
           bAffectedByHandbrake ? TEXT("Enabled") : TEXT("Disabled"));
}
