// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllableVehicle.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UControllableVehicle : public UInterface
{
    GENERATED_BODY()
};

/**
 * The "Contract" for any object that can be driven by our player controller.
 * It defines the functions a vehicle MUST have.
 */
class SRTT_API IControllableVehicle
{
    GENERATED_BODY()

public:
	// All controllable vehicles must be able to apply throttle.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void ApplyThrottle(float Value);

	// All controllable vehicles must be able to apply steering.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void ApplySteer(float Value);

	/** Signals the vehicle to shift to the next gear */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void ShiftGearUp();

	/** Signals the vehicle to shift to the previous gear */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void ShiftGearDown();

	// All controllable vehicles must be able to apply brakes.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void ApplyBrake(float Value);

	// All controllable vehicles must have a way to engage/disengage the handbrake.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void SetHandbrake(bool bIsEngaged);

	// All controllable vehicles must respond to camera look input.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
	void ApplyLook(const FVector2D& LookAxisVector);
};

