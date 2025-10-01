// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRTTUserWidget.generated.h"

/**
 * Base User Widget for SRTT HUD elements
 */
UCLASS()
class SRTT_API USRTTUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    /** Called from Blueprint to update HUD values */
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateSpeed(float Kmh);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateHealth(float Health);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateAmmo(int32 Ammo);

    // New functions for gear, rpm, and speed
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateGear(int32 Gear);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateRPM(int32 RPM);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdateSpeedInt(int32 Speed);
};