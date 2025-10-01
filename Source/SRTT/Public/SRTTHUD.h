// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SRTTUserWidget.h"
#include "SRTTHUD.generated.h"

/**
 * 
 */
UCLASS()
class SRTT_API ASRTTHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void DrawHUD() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Getter for the current widget
    USRTTUserWidget* GetCurrentWidget() const { return CurrentWidget; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class USRTTUserWidget> HUDWidgetClass;

    UPROPERTY()
    USRTTUserWidget* CurrentWidget;
	
};
