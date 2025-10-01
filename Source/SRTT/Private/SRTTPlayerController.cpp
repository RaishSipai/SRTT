// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTPlayerController.h"
#include "SRTTWheeledVehiclePawn.h"
#include "Engine/Engine.h"

void ASRTTPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind analog input actions (0.0 to 1.0) - both Triggered and Completed
        if (ThrottleAction)
        {
            EIC->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::OnThrottle);
            EIC->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::OnThrottleCompleted);
        }
        if (BrakeAction)
        {
            EIC->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::OnBrake);
            EIC->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::OnBrakeCompleted);
        }
        if (SteerAction)
        {
            EIC->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::OnSteer);
            EIC->BindAction(SteerAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::OnSteerCompleted);
        }
        if (ClutchAction)
        {
            EIC->BindAction(ClutchAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::OnClutch);
            EIC->BindAction(ClutchAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::OnClutchCompleted);
        }
        if (HandbrakeAction)
        {
            EIC->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::OnHandbrake);
            EIC->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::OnHandbrakeCompleted);
        }

        // Bind camera input action
        if (AimAction)
            EIC->BindAction(AimAction, ETriggerEvent::Triggered, this, &ASRTTPlayerController::OnAim);

        // Bind boolean input actions
        if (GearUpAction)
            EIC->BindAction(GearUpAction, ETriggerEvent::Started, this, &ASRTTPlayerController::OnGearUp);
        if (GearDownAction)
            EIC->BindAction(GearDownAction, ETriggerEvent::Started, this, &ASRTTPlayerController::OnGearDown);
        if (FireAction)
            EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ASRTTPlayerController::OnFire);
        if (UsePowerupAction)
            EIC->BindAction(UsePowerupAction, ETriggerEvent::Started, this, &ASRTTPlayerController::OnUsePowerup);
        if (HornAction)
        {
            EIC->BindAction(HornAction, ETriggerEvent::Started, this, &ASRTTPlayerController::OnHornStart);
            EIC->BindAction(HornAction, ETriggerEvent::Completed, this, &ASRTTPlayerController::OnHornEnd);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast InputComponent to EnhancedInputComponent"));
    }
}

void ASRTTPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("InputMappingContext is null"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get EnhancedInputLocalPlayerSubsystem"));
    }
}

void ASRTTPlayerController::OnThrottle(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        float ThrottleValue = Value.Get<float>();
        
        // Only handle positive values (W key) for throttle
        if (ThrottleValue > 0.0f)
        {
            Veh->SetThrottle(ThrottleValue);
            UE_LOG(LogTemp, Verbose, TEXT("Throttle applied: %f"), ThrottleValue);
        }
        // Handle negative values (S key) for brake
        else if (ThrottleValue < 0.0f)
        {
            Veh->SetThrottle(0.0f);
            Veh->SetBrake(FMath::Abs(ThrottleValue));
            UE_LOG(LogTemp, Verbose, TEXT("Brake applied: %f"), FMath::Abs(ThrottleValue));
        }
        // Handle zero input
        else
        {
            Veh->SetThrottle(0.0f);
            // Don't reset brake here - let brake action handle it
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No vehicle pawn found for throttle input"));
    }
}

void ASRTTPlayerController::OnThrottleCompleted(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Reset throttle to zero when W key is released
        Veh->SetThrottle(0.0f);
        UE_LOG(LogTemp, Log, TEXT("Throttle reset to zero"));
    }
}

void ASRTTPlayerController::OnBrake(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        Veh->SetBrake(Value.Get<float>());
    }
}

void ASRTTPlayerController::OnBrakeCompleted(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Reset brake to zero when key is released
        Veh->SetBrake(0.0f);
        UE_LOG(LogTemp, Log, TEXT("Brake reset to zero"));
    }
}

void ASRTTPlayerController::OnSteer(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        Veh->SetSteer(Value.Get<float>());
    }
}

void ASRTTPlayerController::OnSteerCompleted(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Reset steering to center when key is released
        Veh->SetSteer(0.0f);
        UE_LOG(LogTemp, Log, TEXT("Steering reset to center"));
    }
}

void ASRTTPlayerController::OnClutch(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        float ClutchValue = Value.Get<float>();
        bool bWasClutchPressed = bClutchPressed;
        bClutchPressed = ClutchValue > 0.1f; // Threshold for clutch pressed

        // Store current gear when clutch is first pressed
        if (bClutchPressed && !bWasClutchPressed)
        {
            ClutchMemoryGear = Veh->GetCurrentGear();
            UE_LOG(LogTemp, Log, TEXT("Clutch pressed, storing gear: %d"), ClutchMemoryGear);
        }

        // When clutch is released, restore the remembered gear
        if (!bClutchPressed && bWasClutchPressed)
        {
            if (Veh->GetCurrentGear() == 0) // Only if currently in neutral
            {
                // Update the Chaos vehicle movement component
                if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(Veh->GetVehicleMovementComponent()))
                {
                    MoveComp->SetTargetGear(ClutchMemoryGear, true);
                    MoveComp->SetUseAutomaticGears(false);
                }
                
                UE_LOG(LogTemp, Log, TEXT("Clutch released, restoring gear: %d"), ClutchMemoryGear);
            }
        }

        Veh->OnClutch(ClutchValue);
    }
}

void ASRTTPlayerController::OnClutchCompleted(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Reset clutch to zero when key is released
        Veh->OnClutch(0.0f);
        bClutchPressed = false;
        UE_LOG(LogTemp, Log, TEXT("Clutch reset to zero"));
    }
}

void ASRTTPlayerController::OnHandbrake(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Handbrake is analog (0.0 to 1.0)
        float HandbrakeValue = Value.Get<float>();
        Veh->SetHandbrake(HandbrakeValue);
    }
}

void ASRTTPlayerController::OnHandbrakeCompleted(const FInputActionValue& Value)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Reset handbrake to zero when key is released
        Veh->SetHandbrake(0.0f);
        UE_LOG(LogTemp, Log, TEXT("Handbrake reset to zero"));
    }
}

void ASRTTPlayerController::OnAim(const FInputActionValue& Value)
{
    // Handle camera movement (mouse XY or gamepad right stick)
    FVector2D AimValue = Value.Get<FVector2D>();
    
    // Apply sensitivity
    AimValue *= CameraSensitivity;
    
    // Mouse Y controls camera pitch (Z rotation), Mouse X controls camera yaw (Y rotation)
    // Y axis is NOT inverted: up = up, down = down
    float PitchInput = AimValue.Y;
    float YawInput = AimValue.X;
    
    // Apply camera rotation
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        if (USpringArmComponent* SpringArm = Veh->GetSpringArm())
        {
            // Get current rotation
            FRotator CurrentRotation = SpringArm->GetRelativeRotation();
            
            // Calculate new rotation
            float NewPitch = FMath::Clamp(CurrentRotation.Pitch + PitchInput, CameraPitchMin, CameraPitchMax);
            float NewYaw = CurrentRotation.Yaw + YawInput;
            
            // Apply new rotation
            SpringArm->SetRelativeRotation(FRotator(NewPitch, NewYaw, 0.0f));
        }
    }
}

void ASRTTPlayerController::OnGearUp(const FInputActionValue&)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Only allow gear change if clutch is pressed
        if (bClutchPressed)
        {
            Veh->ShiftUp();
            // Update clutch memory gear
            ClutchMemoryGear = Veh->GetCurrentGear();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot shift up: clutch not pressed"));
            if (ClutchFailFeedback)
                ClientPlayForceFeedback(ClutchFailFeedback);
            if (ClutchFailCameraShake)
                ClientStartCameraShake(ClutchFailCameraShake);
        }
    }
}

void ASRTTPlayerController::OnGearDown(const FInputActionValue&)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // Only allow gear change if clutch is pressed
        if (bClutchPressed)
        {
            Veh->ShiftDown();
            // Update clutch memory gear
            ClutchMemoryGear = Veh->GetCurrentGear();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot shift down: clutch not pressed"));
            if (ClutchFailFeedback)
                ClientPlayForceFeedback(ClutchFailFeedback);
            if (ClutchFailCameraShake)
                ClientStartCameraShake(ClutchFailCameraShake);
        }
    }
}

void ASRTTPlayerController::OnFire(const FInputActionValue&)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        Veh->FireWeapon();
    }
}

void ASRTTPlayerController::OnUsePowerup(const FInputActionValue&)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        // TODO: Implement powerup usage
        UE_LOG(LogTemp, Log, TEXT("Use Powerup triggered"));
    }
}

void ASRTTPlayerController::OnHornStart(const FInputActionValue&)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        Veh->ActivateHorn();
    }
}

void ASRTTPlayerController::OnHornEnd(const FInputActionValue&)
{
    if (ASRTTWheeledVehiclePawn* Veh = Cast<ASRTTWheeledVehiclePawn>(GetPawn()))
    {
        Veh->DeactivateHorn();
    }
}
