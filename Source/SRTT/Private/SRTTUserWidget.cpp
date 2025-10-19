// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTUserWidget.h"
#include "GameFramework/Pawn.h"

void USRTTUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Cache the pawn and its interface
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (OwningPawn && OwningPawn->Implements<UVehicleDataInterface>())
	{
		VehicleDataSource = OwningPawn;
	}
}

void USRTTUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Check if our data source is valid
	if (VehicleDataSource)
	{
		// --- Pull Speed ---
		const float Speed = IVehicleDataInterface::Execute_GetSpeedKPH(VehicleDataSource.GetObject());
		if (SpeedText)
		{
			// Format as an integer
			SpeedText->SetText(FText::AsNumber(FMath::RoundToInt(Speed)));
		}

		// --- Pull Gear ---
		const FString Gear = IVehicleDataInterface::Execute_GetGearAsString(VehicleDataSource.GetObject());
		if (GearText)
		{
			GearText->SetText(FText::FromString(Gear));
		}

		//// --- Pull RPM ---
		//const float RPM = IVehicleDataInterface::Execute_GetEngineRPM(VehicleDataSource.GetObject());
		//// (Use this float value to update your analog RPM meter)

		CurrentRPM = IVehicleDataInterface::Execute_GetEngineRPM(VehicleDataSource.GetObject());
		if (RpmProgressBar)
		{
			// Assuming max RPM is 8000
			const float NormalizedRPM = FMath::Clamp(CurrentRPM / MaxEngineRPM, 0.0f, 1.0f);
			RpmProgressBar->SetPercent(NormalizedRPM);
		}
	}
	else
	{
		// If our pawn was destroyed or changed, try to get it again
		APawn* OwningPawn = GetOwningPlayerPawn();
		if (OwningPawn && OwningPawn->Implements<UVehicleDataInterface>())
		{
			VehicleDataSource = OwningPawn;
		}
	}
}

