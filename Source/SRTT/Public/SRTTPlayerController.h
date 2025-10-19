// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SRTTPlayerController.generated.h"


// Forward declarations to avoid including heavy headers here. This is good practice.
class IControllableVehicle;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UUserWidget;

/**
 * 
 */
UCLASS()
class SRTT_API ASRTTPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	// Called when the controller possesses a pawn.
	virtual void OnPossess(APawn* InPawn) override;

	// Called to bind functionality to input.
	virtual void SetupInputComponent() override;

	// --- INPUT ASSETS ---
	// Assign these in your Blueprint child class of this controller.
	// 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* VehicleMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ThrottleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SteerAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GearUpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GearDownAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* BrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* HandbrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** The Widget Blueprint class to use for the vehicle HUD */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> VehicleHUDClass;

	/** The instance of the Vehicle HUD widget */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UUserWidget> VehicleHUDInstance;

private:
	// --- INPUT HANDLER FUNCTIONS ---

	void HandleThrottle(const FInputActionValue& ActionValue);
	void HandleSteer(const FInputActionValue& ActionValue);
	void HandleGearUp(const FInputActionValue& ActionValue);
	void HandleGearDown(const FInputActionValue& ActionValue);
	void HandleBrake(const FInputActionValue& ActionValue);
	void HandleHandbrakePressed(const FInputActionValue& ActionValue);
	void HandleHandbrakeReleased(const FInputActionValue& ActionValue);
	void HandleLook(const FInputActionValue& ActionValue);

	// The pointer to the pawn we are controlling, referenced only by its "contract".
	// This is the key to decoupling.
	TScriptInterface<IControllableVehicle> PossessedVehicle;
	
};
