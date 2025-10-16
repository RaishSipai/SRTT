// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVehicleControlInterface.generated.h"

UINTERFACE(Blueprintable)
class SRTT_API USRTTVehicleControlInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface to allow external control (AI or PlayerController) of a vehicle.
 */
class SRTT_API ISRTTVehicleControlInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
    void Accelerate(float Value);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
    void Brake(float Value);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
    void Steer(float Value);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Vehicle Control")
    void UseBoost();
};

