// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SRTTHUD.generated.h"

class USRTTUserWidget;

UCLASS()
class SRTT_API ASRTTHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Returns the active HUD widget. */
	UFUNCTION(BlueprintPure, Category = "HUD")
	USRTTUserWidget* GetHUDWidget() const { return HUDWidget; }

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// This property will hold the Blueprint class of the widget we want to create.
	// We will set this in the Blueprint editor.
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<USRTTUserWidget> HUDWidgetClass;

private:
	// The actual instance of our HUD widget that is created at runtime.
	UPROPERTY()
	TObjectPtr<USRTTUserWidget> HUDWidget;
};
