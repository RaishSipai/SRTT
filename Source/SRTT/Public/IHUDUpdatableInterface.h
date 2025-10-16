// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHUDUpdatableInterface.generated.h"

UINTERFACE(Blueprintable)
class SRTT_API USRTTHUDUpdatableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for updating HUD elements (health, ammo, speed, etc.).
 */
class SRTT_API ISRTTHUDUpdatableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void UpdateSpeed(float CurrentSpeed, float MaxSpeed);
};

