// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRTTGearboxComponent.generated.h"

// --- DELEGATES for event-driven communication ---
// Broadcasts when the current gear changes. The UI will listen to this.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearChangedSignature, int32, NewGear);

// Broadcasts when a gear shift is attempted without the clutch engaged. The Player Controller will listen to this for feedback.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShiftFailedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SRTT_API USRTTGearboxComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRTTGearboxComponent();

	// --- DELEGATE PROPERTIES ---
	// The event broadcast when the gear is successfully changed.
	UPROPERTY(BlueprintAssignable, Category = "Gearbox|Events")
	FOnGearChangedSignature OnGearChanged;

	// The event broadcast when a shift fails.
	UPROPERTY(BlueprintAssignable, Category = "Gearbox|Events")
	FOnShiftFailedSignature OnShiftFailed;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// --- DYNAMIC CONFIGURATION (The Single Source of Truth) ---
	// These properties are marked 'EditDefaultsOnly' so they can be configured in the Blueprint editor for each vehicle type.

	/** * The gear ratios for the transmission. This is the master list.
	 * Index 0 = Reverse Gear Ratio
	 * Index 1 = 1st Forward Gear Ratio
	 * ...and so on.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gearbox|Configuration")
	TArray<float> GearRatios;

	/** The final drive ratio, which multiplies all gear ratios. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gearbox|Configuration")
	float FinalDriveRatio;

	/** The clutch input value (0-1) required to be able to shift gears. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gearbox|Configuration", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float ClutchShiftThreshold;

private:
	// --- STATE ---
	// The current gear the transmission is in. ( -1 = Reverse, 0 = Neutral, 1 = 1st, etc. )
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gearbox|State", meta = (AllowPrivateAccess = "true"))
	int32 CurrentGear;

	// The current position of the clutch pedal (0.0 = engaged, 1.0 = fully pressed).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gearbox|State", meta = (AllowPrivateAccess = "true"))
	float ClutchPosition;

	// A cached pointer to the vehicle's movement component for performance.
	UPROPERTY()
	TObjectPtr<class UChaosWheeledVehicleMovementComponent> VehicleMovementComponent;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- PUBLIC API (Functions called by the PlayerController) ---

	/** Called by the Player Controller to update the clutch state. */
	void HandleClutchInput(float Value);

	/** Called by the Player Controller to attempt a shift up. */
	void HandleShiftUp();

	/** Called by the Player Controller to attempt a shift down. */
	void HandleShiftDown();

	/** Returns the current gear. */
	UFUNCTION(BlueprintPure, Category = "Gearbox")
	int32 GetCurrentGear() const { return CurrentGear; }

	/** Returns the current clutch position. */
	UFUNCTION(BlueprintPure, Category = "Gearbox")
	float GetClutchPosition() const { return ClutchPosition; }

		
};
