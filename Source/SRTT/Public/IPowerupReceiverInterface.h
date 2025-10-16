// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPowerupReceiverInterface.generated.h"

UINTERFACE(Blueprintable)
class SRTT_API USRTTPowerupReceiverInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for actors that can receive and react to powerups.
 */
class SRTT_API ISRTTPowerupReceiverInterface
{
    GENERATED_BODY()

public:
    /** Apply the specified power-up effect */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Powerup")
    void ApplyPowerupEffect(FName PowerupType, float Duration);
};

