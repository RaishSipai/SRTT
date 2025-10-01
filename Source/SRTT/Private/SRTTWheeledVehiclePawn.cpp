// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTTWheeledVehiclePawn.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SRTTProjectile.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "SRTTHUD.h"
#include "SRTTUserWidget.h"

ASRTTWheeledVehiclePawn::ASRTTWheeledVehiclePawn()
{
    // Enable physics simulation
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetEnableGravity(true);
    
    // Set physics properties
    GetMesh()->SetMassOverrideInKg("Root", 1500.0f, true);
    GetMesh()->SetLinearDamping(0.1f);
    GetMesh()->SetAngularDamping(0.1f);
    
    // Enable collision
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetMesh());
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    WeaponMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponMuzzle"));
    WeaponMuzzle->SetupAttachment(GetMesh());
    // Set default weapon muzzle position (can be adjusted in Blueprint)
    WeaponMuzzle->SetRelativeLocation(FVector(200.f, 0.f, 50.f));

    // Create horn audio component
    HornAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HornAudioComponent"));
    HornAudioComponent->SetupAttachment(GetMesh());
    HornAudioComponent->bAutoActivate = false;

    // Ensure movement component is Chaos-based and properly configured
    UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
    if (MoveComp)
    {
        MoveComp->Mass = 1500.f;
        
        // Enable engine and transmission
        MoveComp->EngineSetup.MaxTorque = 500.0f;
        MoveComp->EngineSetup.MaxRPM = 6000.0f;
        MoveComp->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
        MoveComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
        MoveComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
        MoveComp->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);
        
        // Transmission setup - use only available properties
        MoveComp->TransmissionSetup.bUseAutomaticGears = false;
        
        // CRITICAL: Force manual transmission mode
        MoveComp->SetUseAutomaticGears(false);
        
        // Differential setup
        MoveComp->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
        MoveComp->DifferentialSetup.FrontRearSplit = 0.5f;
        
        // Additional debugging for engine setup
        UE_LOG(LogTemp, Log, TEXT("Engine Setup - Max Torque: %f, Max RPM: %f"), 
               MoveComp->EngineSetup.MaxTorque, MoveComp->EngineSetup.MaxRPM);
        UE_LOG(LogTemp, Log, TEXT("Torque Curve Points: %d"), 
               MoveComp->EngineSetup.TorqueCurve.GetRichCurve()->GetNumKeys());
        
        UE_LOG(LogTemp, Log, TEXT("Vehicle Movement Component configured for manual transmission"));
    }

    // Initialize transmission variables
    CurrentGear = 0;
    ClutchPosition = 0.0f;
    GearRatios = { 0.0f, 2.5f, 1.8f, 1.3f, 1.0f, 0.8f };
    MaxGears = 5;
}

void ASRTTWheeledVehiclePawn::BeginPlay()
{
    Super::BeginPlay();
    
    // Ensure physics is enabled
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetEnableGravity(true);
    
    // Set initial gear to neutral
    CurrentGear = 0;
    
    // Verify vehicle movement component setup
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        UE_LOG(LogTemp, Log, TEXT("Vehicle Movement Component found"));
        UE_LOG(LogTemp, Log, TEXT("Number of wheels: %d"), MoveComp->WheelSetups.Num());
        UE_LOG(LogTemp, Log, TEXT("Engine Max Torque: %f"), MoveComp->EngineSetup.MaxTorque);
        UE_LOG(LogTemp, Log, TEXT("Engine Max RPM: %f"), MoveComp->EngineSetup.MaxRPM);
        UE_LOG(LogTemp, Log, TEXT("Transmission Auto Gears: %s"), MoveComp->TransmissionSetup.bUseAutomaticGears ? TEXT("True") : TEXT("False"));
        
        // Check if wheels are properly configured
        for (int32 i = 0; i < MoveComp->WheelSetups.Num(); i++)
        {
            UE_LOG(LogTemp, Log, TEXT("Wheel %d: Bone Name = %s"), i, *MoveComp->WheelSetups[i].BoneName.ToString());
        }
        
        // Ensure the vehicle is ready to drive
        MoveComp->SetTargetGear(0, true); // Start in neutral
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get ChaosWheeledVehicleMovementComponent"));
    }
    
    // Log initial setup
    UE_LOG(LogTemp, Log, TEXT("Vehicle spawned with physics enabled. Mass: %f"), GetMesh()->GetMass());
}

//void ASRTTWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
    //Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

void ASRTTWheeledVehiclePawn::SetThrottle(float Value)
{
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        // Apply throttle input to the movement component
        MoveComp->SetThrottleInput(Value);
        
        // Only log significant changes or errors
        if (Value > 0.0f && CurrentGear == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Throttle applied in neutral gear - vehicle won't move"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to cast to ChaosWheeledVehicleMovementComponent"));
    }
    UpdateHUD();
}

void ASRTTWheeledVehiclePawn::SetBrake(float Value)
{
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        MoveComp->SetBrakeInput(Value);
    }
}

void ASRTTWheeledVehiclePawn::SetSteer(float Value)
{
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        MoveComp->SetSteeringInput(Value);
    }
}

void ASRTTWheeledVehiclePawn::SetHandbrake(float Value)
{
    HandbrakeInput = FMath::Clamp(Value, 0.0f, 1.0f);
    
    // Store handbrake input for the vehicle movement component
    // The Chaos Vehicle Movement Component will handle handbrake automatically
    // based on the bAffectedByHandbrake flag on rear wheels
        
        if (HandbrakeInput > 0.0f)
        {
        UE_LOG(LogTemp, Verbose, TEXT("Handbrake applied: %f"), HandbrakeInput);
    }
}

void ASRTTWheeledVehiclePawn::ShiftUp() 
{ 
    // Only allow shift if clutch is pressed beyond threshold
    if (ClutchPosition >= ClutchShiftThreshold && CurrentGear < MaxGears)
    {
        int32 PreviousGear = CurrentGear;
        CurrentGear++;
        
        // Update the Chaos vehicle movement component with the new gear
        if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
        {
            // Only update Chaos gear if clutch is released (engaged)
            if (ClutchPosition <= KINDA_SMALL_NUMBER)
            {
                MoveComp->SetTargetGear(CurrentGear, true);
                MoveComp->SetUseAutomaticGears(false);
            }
            
            UE_LOG(LogTemp, Log, TEXT("ShiftUp: Previous=%d, New=%d, ChaosGear=%d, Clutch=%f"), 
                   PreviousGear, CurrentGear, MoveComp->GetTargetGear(), ClutchPosition);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot shift up: clutch=%f, currentGear=%d, maxGears=%d"), 
               ClutchPosition, CurrentGear, MaxGears);
    }
    UpdateHUD();
}

void ASRTTWheeledVehiclePawn::ShiftDown() 
{ 
    // Only allow shift if clutch is pressed beyond threshold
    if (ClutchPosition >= ClutchShiftThreshold && CurrentGear > 0)
    {
        int32 PreviousGear = CurrentGear;
        CurrentGear--;
        
        // Update the Chaos vehicle movement component with the new gear
        if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
        {
            // Only update Chaos gear if clutch is released (engaged)
            if (ClutchPosition <= KINDA_SMALL_NUMBER)
            {
                MoveComp->SetTargetGear(CurrentGear, true);
                MoveComp->SetUseAutomaticGears(false);
            }
            
            UE_LOG(LogTemp, Log, TEXT("ShiftDown: Previous=%d, New=%d, ChaosGear=%d, Clutch=%f"), 
                   PreviousGear, CurrentGear, MoveComp->GetTargetGear(), ClutchPosition);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot shift down: clutch=%f, currentGear=%d"), 
               ClutchPosition, CurrentGear);
    }
    UpdateHUD();
}

void ASRTTWheeledVehiclePawn::OnClutch(float Value)
{
    float PreviousClutchValue = ClutchPosition;
    ClutchPosition = FMath::Clamp(Value, 0.0f, 1.0f);

    // When clutch is pressed, go to neutral
    if (ClutchPosition > KINDA_SMALL_NUMBER)
    {
        // Set Chaos vehicle to neutral
        if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
        {
            MoveComp->SetTargetGear(0, true);
            MoveComp->SetUseAutomaticGears(false);
        }
    }
    // When clutch is released, go back to current gear
    else
    {
        // Set Chaos vehicle to current gear
        if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
        {
            MoveComp->SetTargetGear(CurrentGear, true);
            MoveComp->SetUseAutomaticGears(false);
        }
    }
    UpdateHUD();
}

void ASRTTWheeledVehiclePawn::FireWeapon()
{
    if (!WeaponMuzzle || !ProjectileClass) return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    FVector SpawnLocation = WeaponMuzzle->GetComponentLocation();
    FRotator SpawnRotation;
    FVector AimDirection = GetActorForwardVector(); // Default

    if (PC)
    {
        // Get the camera location and direction
        FVector CameraLocation;
        FRotator CameraRotation;
        PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
        AimDirection = CameraRotation.Vector();
        SpawnRotation = CameraRotation;
    }
    else
    {
        SpawnRotation = WeaponMuzzle->GetComponentRotation();
    }

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();

    ASRTTProjectile* Proj = GetWorld()->SpawnActor<ASRTTProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
    if (Proj)
    {
        // Add vehicle's velocity to projectile's velocity
        FVector VehicleVelocity = GetVelocity();
        FVector InitialVelocity = AimDirection * ProjectileSpeed + VehicleVelocity;
        Proj->InitProjectile(InitialVelocity);
        UE_LOG(LogTemp, Log, TEXT("Projectile fired from weapon muzzle in aim direction"));
    }
}

void ASRTTWheeledVehiclePawn::ActivateHorn()
{
    if (HornAudioComponent && HornSound)
    {
        // Stop any currently playing horn sound
        HornAudioComponent->Stop();
        
        // Set the horn sound and play it
        HornAudioComponent->SetSound(HornSound);
        HornAudioComponent->SetVolumeMultiplier(HornVolume);
        HornAudioComponent->SetPitchMultiplier(HornPitch);
        HornAudioComponent->Play();
        
        UE_LOG(LogTemp, Log, TEXT("Horn activated - Volume: %f, Pitch: %f"), HornVolume, HornPitch);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot activate horn: Audio component or sound not found"));
    }
}

void ASRTTWheeledVehiclePawn::DeactivateHorn()
{
    if (HornAudioComponent)
    {
        HornAudioComponent->Stop();
        UE_LOG(LogTemp, Log, TEXT("Horn deactivated"));
    }
}

// Helper function to update HUD with gear, RPM, and speed
void ASRTTWheeledVehiclePawn::UpdateHUD()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;
    ASRTTHUD* HUD = Cast<ASRTTHUD>(PC->GetHUD());
    if (!HUD) return;
    USRTTUserWidget* Widget = HUD->GetCurrentWidget();
    if (!Widget) return;

    // Get current speed and RPM from movement component
    int32 Gear = CurrentGear;
    int32 RPM = 0;
    int32 Speed = 0;
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        RPM = FMath::RoundToInt(MoveComp->GetEngineRotationSpeed());
        Speed = FMath::RoundToInt(MoveComp->GetForwardSpeed() * 0.036f); // cm/s to km/h
    }
    Widget->UpdateGear(Gear);
    Widget->UpdateRPM(RPM);
    Widget->UpdateSpeedInt(Speed);
}

