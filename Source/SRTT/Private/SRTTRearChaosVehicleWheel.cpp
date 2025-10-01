// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTRearChaosVehicleWheel.h"
#include "UObject/ConstructorHelpers.h"

USRTTRearChaosVehicleWheel::USRTTRearChaosVehicleWheel()
{
    // Rear wheel configuration for engine power, handbrake, and braking
    
    // Disable steering for rear wheels
    bAffectedBySteering = false;
    
    // Enable braking for rear wheels
    bAffectedByBrake = true;
    
    // Enable engine drive for rear wheels (rear-wheel drive)
    bAffectedByEngine = true;
    
    // Enable handbrake for rear wheels
    bAffectedByHandbrake = true;
    
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
    
    // Brake settings
    MaxBrakeTorque = 1500.0f;
    
    // Wheel mass and inertia
    WheelMass = 20.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Rear wheel configured - Steering: %s, Brake: %s, Engine: %s, Handbrake: %s"), 
           bAffectedBySteering ? TEXT("Enabled") : TEXT("Disabled"),
           bAffectedByBrake ? TEXT("Enabled") : TEXT("Disabled"),
           bAffectedByEngine ? TEXT("Enabled") : TEXT("Disabled"),
           bAffectedByHandbrake ? TEXT("Enabled") : TEXT("Disabled"));
}
