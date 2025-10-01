// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "SRTTProjectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "SRTTWheeledVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class SRTT_API ASRTTWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
    ASRTTWheeledVehiclePawn();

    virtual void BeginPlay() override;

    //virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void SetThrottle(float Value);
    void SetBrake(float Value);
    void SetSteer(float Value);

    void ShiftUp();
    void ShiftDown();

    /** Called by Controller when clutch axis moves */
    void OnClutch(float Value);

    void FireWeapon();

    /** Activate horn sound */
    void ActivateHorn();

    /** Deactivate horn sound */
    void DeactivateHorn();

    /** Allow Controller to access SpringArm for camera control */
    USpringArmComponent* GetSpringArm() const { return SpringArm; }

    /** Set handbrake input (0.0 to 1.0) */
    void SetHandbrake(float Value);

    /** Get current gear */
    int32 GetCurrentGear() const { return CurrentGear; }

    /** Get clutch position */
    float GetClutchPosition() const { return ClutchPosition; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    USceneComponent* WeaponMuzzle;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TSubclassOf<ASRTTProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (ClampMin = "100.0", ClampMax = "5000.0"))
    float ProjectileSpeed = 1500.f;

    /** Current handbrake input value */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float HandbrakeInput = 0.0f;

    /** Maximum handbrake force applied to rear wheels */
    UPROPERTY(EditDefaultsOnly, Category = "Vehicle", meta = (ClampMin = "100.0", ClampMax = "5000.0"))
    float MaxHandbrakeForce = 2000.0f;

    /** Horn sound component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UAudioComponent* HornAudioComponent;

    /** Horn sound asset */
    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    class USoundBase* HornSound;

    /** Horn volume */
    UPROPERTY(EditDefaultsOnly, Category = "Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HornVolume = 0.8f;

    /** Horn pitch */
    UPROPERTY(EditDefaultsOnly, Category = "Audio", meta = (ClampMin = "0.5", ClampMax = "2.0"))
    float HornPitch = 1.0f;

    /** Manual transmission variables */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transmission")
    int32 CurrentGear = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transmission")
    float ClutchPosition = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Transmission")
    float ClutchShiftThreshold = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "Transmission")
    TArray<float> GearRatios = { 0.0f, 2.5f, 1.8f, 1.3f, 1.0f, 0.8f };

    /** Maximum number of gears */
    UPROPERTY(EditDefaultsOnly, Category = "Transmission")
    int32 MaxGears = 5;

    void UpdateHUD();
};
