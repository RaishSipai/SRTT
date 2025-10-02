// Copyright SRTT Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SRTTPlayerController.generated.h"

// Forward declare input classes
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
// Forward declare feedback classes
class UCameraShakeBase;
class UForceFeedbackEffect;

UCLASS()
class SRTT_API ASRTTPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// --- INPUT ASSETS ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// --- Axis Inputs ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ThrottleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SteerAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> BrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> HandbrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ClutchAction;

	// --- Action Inputs ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GearUpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GearDownAction;

private:
	// --- INPUT HANDLER FUNCTIONS ---
	// Driving
	void HandleThrottle(const FInputActionValue& Value);
	void HandleSteer(const FInputActionValue& Value);
	void HandleBrake(const FInputActionValue& Value);
	void HandleHandbrake(const FInputActionValue& Value);

	// Handlers for when input keys are released
	void HandleThrottleReleased(const FInputActionValue& Value);
	void HandleSteerReleased(const FInputActionValue& Value);
	void HandleBrakeReleased(const FInputActionValue& Value);
	void HandleHandbrakeReleased(const FInputActionValue& Value);

	// Manual Transmission
	void HandleClutch(const FInputActionValue& Value);
	void HandleGearUp(const FInputActionValue& Value);
	void HandleGearDown(const FInputActionValue& Value);

	// --- FEEDBACK ---
	UPROPERTY(EditDefaultsOnly, Category = "Input|Feedback")
	TSubclassOf<UCameraShakeBase> ShiftFailCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Feedback")
	TObjectPtr<UForceFeedbackEffect> ShiftFailForceFeedback;

	UFUNCTION()
	void OnShiftFailed();
};

