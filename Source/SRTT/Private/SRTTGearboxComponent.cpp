// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTGearboxComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

// Sets default values for this component's properties
USRTTGearboxComponent::USRTTGearboxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Set up safe, default values in C++. These are meant to be overridden in a Blueprint subclass.
	FinalDriveRatio = 3.42f;
	ClutchShiftThreshold = 0.9f;
	CurrentGear = 0; // Start in Neutral
	ClutchPosition = 0.0f;

	// Default gear ratios. Index 0=Reverse, 1=1st Gear, etc.
	GearRatios = { -2.9f, 2.66f, 1.78f, 1.3f, 1.0f, 0.74f, 0.5f };
}


// Called when the game starts
void USRTTGearboxComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the owner actor and cache its vehicle movement component for performance.
	AActor* Owner = GetOwner();
	if (Owner)
	{
		VehicleMovementComponent = Owner->FindComponentByClass<UChaosWheeledVehicleMovementComponent>();
	}

	if (!VehicleMovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("SRTTGearboxComponent on %s requires a UChaosWheeledVehicleMovementComponent. Disabling component."), *GetNameSafe(Owner));
		SetComponentTickEnabled(false); // Disable this component if it can't function
		return;
	}

	// --- CRITICAL: DATA SYNCHRONIZATION ---
	// Here, we establish this component as the "Single Source of Truth".
	// We take our configured data and force it onto the physics component to prevent data mismatch bugs.

	// 1. Set the Transmission Type to Manual
	VehicleMovementComponent->SetUseAutomaticGears(false);

	// 2. Clear any existing gear setups on the physics component to prevent conflicts.
	VehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Empty();
	VehicleMovementComponent->TransmissionSetup.ReverseGearRatios.Empty();

	// 3. Rebuild the physics component's gear data from our own TArray.
	if (GearRatios.Num() > 0)
	{
		// Add the reverse gear (at index 0)
		VehicleMovementComponent->TransmissionSetup.ReverseGearRatios.Add(GearRatios[0]);
	}
	// Add all forward gears (from index 1 onwards)
	for (int32 i = 1; i < GearRatios.Num(); ++i)
	{
		VehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Add(GearRatios[i]);
	}

	// 4. Set the Final Drive Ratio
	VehicleMovementComponent->TransmissionSetup.FinalRatio = FinalDriveRatio;

	UE_LOG(LogTemp, Log, TEXT("SRTTGearboxComponent synchronized its data with the ChaosWheeledVehicleMovementComponent."));

	// Broadcast the initial gear state for the UI to display "N" correctly.
	OnGearChanged.Broadcast(CurrentGear);
	
}


// Called every frame
void USRTTGearboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USRTTGearboxComponent::HandleClutchInput(float Value)
{
	ClutchPosition = FMath::Clamp(Value, 0.0f, 1.0f);

	if (!VehicleMovementComponent) return;

	// This is the correct way to simulate a clutch with the Chaos Vehicle system.
	// We do NOT call a "SetClutchInput" function.

	// If the clutch is pressed, we tell the physics to shift to Neutral (Gear 0).
	// This decouples the engine from the wheels, allowing it to rev freely.
	if (ClutchPosition >= ClutchShiftThreshold)
	{
		VehicleMovementComponent->SetTargetGear(0, true); // Chaos Neutral is gear 0
	}
	// If the clutch is released, we tell the physics to engage our currently selected gear.
	// This re-engages the drivetrain and provides the "kick".
	else
	{
		// Chaos uses negative for reverse, 0 for neutral, positive for forward. Our CurrentGear variable matches this.
		VehicleMovementComponent->SetTargetGear(CurrentGear, true);
	}
}

void USRTTGearboxComponent::HandleShiftUp()
{
	// Check if the clutch is pressed enough to allow a shift
	if (ClutchPosition >= ClutchShiftThreshold)
	{
		// GearRatios.Num() - 1 is the total number of forward gears.
		const int32 MaxForwardGears = GearRatios.Num() - 1;
		if (CurrentGear < MaxForwardGears)
		{
			CurrentGear++;
			// Broadcast the successful gear change event for UI and audio to hear
			OnGearChanged.Broadcast(CurrentGear);
		}
		else
		{
			// We are at the max gear, so the shift fails
			OnShiftFailed.Broadcast();
		}
	}
	else
	{
		// Clutch was not pressed, broadcast the shift failed event for feedback (camera shake, sound, etc.)
		OnShiftFailed.Broadcast();
	}
}

void USRTTGearboxComponent::HandleShiftDown()
{
	// Check if the clutch is pressed enough to allow a shift
	if (ClutchPosition >= ClutchShiftThreshold)
	{
		// Check if we are not already in Reverse (gear -1)
		if (CurrentGear > -1)
		{
			CurrentGear--;
			// Broadcast the successful gear change event
			OnGearChanged.Broadcast(CurrentGear);
		}
		else
		{
			// We are at the lowest gear, so the shift fails
			OnShiftFailed.Broadcast();
		}
	}
	else
	{
		// Clutch was not pressed, broadcast the shift failed event
		OnShiftFailed.Broadcast();
	}
}
