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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GearUpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> GearDownAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	// --- FEEDBACK ---
	UPROPERTY(EditDefaultsOnly, Category = "Input|Feedback")
	TSubclassOf<UCameraShakeBase> ShiftFailCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Feedback")
	TObjectPtr<UForceFeedbackEffect> ShiftFailForceFeedback;

private:
	// --- INPUT HANDLER FUNCTIONS ---
	// Driving
	void HandleThrottle(const FInputActionValue& Value);
	void HandleSteer(const FInputActionValue& Value);
	void HandleBrake(const FInputActionValue& Value);
	void HandleHandbrake(const FInputActionValue& Value);

	// Handlers for when input keys are released
	void HandleThrottleCompleted(const FInputActionValue& Value);
	void HandleSteerCompleted(const FInputActionValue& Value);
	void HandleBrakeCompleted(const FInputActionValue& Value);
	void HandleHandbrakeCompleted(const FInputActionValue& Value);
	void HandleClutchCompleted(const FInputActionValue& Value);

	// Manual Transmission
	void HandleClutch(const FInputActionValue& Value);
	void HandleGearUp(const FInputActionValue& Value);
	void HandleGearDown(const FInputActionValue& Value);

	void HandleLook(const FInputActionValue& Value);
	void HandleFireStarted();
	void HandleFireCompleted();

	UFUNCTION()
	void OnShiftFailed();
};

