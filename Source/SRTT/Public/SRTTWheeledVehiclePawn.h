// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ControllableVehicle.h"
#include "VehicleDataInterface.h"
#include "SRTTWheeledVehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UChaosWheeledVehicleMovementComponent;

/**
 * 
 */
UCLASS()
class SRTT_API ASRTTWheeledVehiclePawn : public AWheeledVehiclePawn, public IControllableVehicle, public IVehicleDataInterface
{
	GENERATED_BODY()

public:
	ASRTTWheeledVehiclePawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	// I've changed this to a TObjectPtr for modern UE5 C++ style. It's a safer pointer.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovementComponent;

	/** The current clutch input (0.0 to 1.0) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle State")
	float ClutchInput;

	/** The gear the vehicle was in before the clutch was pressed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle State")
	int32 PreClutchGear;

	/** Is the clutch fully pressed (past the 0.9 threshold)? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle State")
	bool bIsClutchEngaged;

	/** Multiplier for engine responsiveness when in neutral */
	UPROPERTY(EditAnywhere, Category = "Vehicle Setup|Engine")
	float NeutralEngineMOIMultiplier;

	/** Threshold for clutch press to be considered "engaged" */
	UPROPERTY(EditAnywhere, Category = "Vehicle Setup|Clutch")
	float ClutchEngageThreshold;

private:

	/** Internal function to modify engine properties for neutral revving */
	void EngageNeutralEngineState();

	/** Internal function to restore engine properties */
	void DisengageNeutralEngineState();

public:
	// --- IControllableVehicle Interface Implementation ---
	// These are the functions that actually perform the actions defined in the contract.
	// The "_Implementation" suffix is required for BlueprintNativeEvents.
	virtual void ApplyThrottle_Implementation(float Value) override;
	virtual void ApplySteer_Implementation(float Value) override;
	virtual void ShiftGearUp_Implementation() override;
	virtual void ShiftGearDown_Implementation() override;
	virtual void ApplyBrake_Implementation(float Value) override;
	virtual void SetHandbrake_Implementation(bool bIsEngaged) override;
	virtual void ApplyLook_Implementation(const FVector2D& LookAxisVector) override;

	// --- ADD CLUTCH OVERRIDE ---
	virtual void ApplyClutch_Implementation(float Value) override;
	virtual void TriggerFailedShiftEffect_Implementation() override;

	// --- IVehicleDataInterface Interface Implementation ---
	virtual float GetSpeedKPH_Implementation() const override;
	virtual float GetEngineRPM_Implementation() const override;
	virtual FString GetGearAsString_Implementation() const override;

	// Helper function to get the spring arm
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	// Helper function to get the camera
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	
};
