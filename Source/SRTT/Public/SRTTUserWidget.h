// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "VehicleDataInterface.h"
#include "SRTTUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SRTT_API USRTTUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Bind these in your WBP child class
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeedText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GearText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> RpmProgressBar;


	// (Add your RPM meter widget here)

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** The engine's max RPM. Used to calculate the progress bar percentage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Data")
	float MaxEngineRPM = 8000.0f; // <--- 3. ADD THIS (makes it editable in BP)

	// --- DATA FROM C++ ---
	/** The current engine RPM, updated on tick by NativeTick */
	UPROPERTY(BlueprintReadOnly, Category = "Vehicle Data")
	float CurrentRPM;

private:
	// Cache the interface for performance
	UPROPERTY(Transient)
	TScriptInterface<IVehicleDataInterface> VehicleDataSource;
	
};

