// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VehicleDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UVehicleDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * The "Contract" for any object that can provide vehicle telemetry data to the HUD.
 */
class SRTT_API IVehicleDataInterface
{
	GENERATED_BODY()

public:
	/** Returns the vehicle's current speed in Kilometers Per Hour (KM/H) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Data")
	float GetSpeedKPH() const;

	/** Returns the vehicle's current engine RPM */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Data")
	float GetEngineRPM() const;

	/** * Returns the vehicle's current gear as a display-ready string.
	 * (e.g., "R", "N", "1", "2")
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Data")
	FString GetGearAsString() const;
};

