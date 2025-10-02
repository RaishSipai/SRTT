// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRTTUserWidget.generated.h"

UCLASS()
class SRTT_API USRTTUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// These functions can be called from C++ but are implemented in the Blueprint graph.
	// This allows a UI artist to design the look and feel without needing to touch C++ code.

	/** Called to update the gear display on the HUD. */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Updates")
	void UpdateGear(int32 NewGear);

	/** Called to update the speedometer display on the HUD. */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Updates")
	void UpdateSpeed(float NewSpeed);

	/** Called to update the health bar display on the HUD. */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Updates")
	void UpdateHealth(float CurrentHealth, float MaxHealth);
};
