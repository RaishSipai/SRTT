// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SRTTPlayerController.generated.h"

class UForceFeedbackEffect;
class UCameraShakeBase;

/**
 * Processes player input (driving, gear, fire, camera).
 */
UCLASS()
class SRTT_API ASRTTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* InputMappingContext;

    // Analog Input Actions (0.0 to 1.0)
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ThrottleAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* BrakeAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SteerAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ClutchAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* HandbrakeAction;

    // Camera Input Action
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* AimAction;

    // Boolean Input Actions
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* GearUpAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* GearDownAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* FireAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* UsePowerupAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* HornAction;

    /** Feedback assets */
    UPROPERTY(EditDefaultsOnly, Category = "Feedback")
    UForceFeedbackEffect* ClutchFailFeedback;

    UPROPERTY(EditDefaultsOnly, Category = "Feedback")
    TSubclassOf<UCameraShakeBase> ClutchFailCameraShake;

    // Camera control properties
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    float CameraSensitivity = 1.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    float CameraPitchMin = -80.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    float CameraPitchMax = 80.0f;

    // Clutch memory system
    UPROPERTY()
    int32 ClutchMemoryGear = 0;
    
    UPROPERTY()
    bool bClutchPressed = false;

    /** Callbacks */
    void OnThrottle(const FInputActionValue& Value);
    void OnThrottleCompleted(const FInputActionValue& Value);
    void OnBrake(const FInputActionValue& Value);
    void OnBrakeCompleted(const FInputActionValue& Value);
    void OnSteer(const FInputActionValue& Value);
    void OnSteerCompleted(const FInputActionValue& Value);
    void OnClutch(const FInputActionValue& Value);
    void OnClutchCompleted(const FInputActionValue& Value);
    void OnHandbrake(const FInputActionValue& Value);
    void OnHandbrakeCompleted(const FInputActionValue& Value);
    void OnAim(const FInputActionValue& Value);
    void OnGearUp(const FInputActionValue& Value);
    void OnGearDown(const FInputActionValue& Value);
    void OnFire(const FInputActionValue& Value);
    void OnUsePowerup(const FInputActionValue& Value);
    void OnHornStart(const FInputActionValue& Value);
    void OnHornEnd(const FInputActionValue& Value);
};
