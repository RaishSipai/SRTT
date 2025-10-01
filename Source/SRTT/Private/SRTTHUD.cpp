// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTHUD.h"
#include "Blueprint/UserWidget.h"
#include "SRTTUserWidget.h"
#include "SRTTWheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void ASRTTHUD::DrawHUD()
{
    Super::DrawHUD();
    
    // Only create widget once
    if (!CurrentWidget && HUDWidgetClass)
    {
        CurrentWidget = CreateWidget<USRTTUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }

    // --- Draw Gear, RPM, Speed in C++ ---
    APawn* Pawn = GetOwningPawn();
    if (!Pawn) return;
    ASRTTWheeledVehiclePawn* Vehicle = Cast<ASRTTWheeledVehiclePawn>(Pawn);
    if (!Vehicle) return;

    int32 Gear = Vehicle->GetCurrentGear();
    int32 RPM = 0;
    int32 Speed = 0;
    if (UChaosWheeledVehicleMovementComponent* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(Vehicle->GetVehicleMovementComponent()))
    {
        RPM = FMath::RoundToInt(MoveComp->GetEngineRotationSpeed());
        Speed = FMath::RoundToInt(MoveComp->GetForwardSpeed() * 0.036f); // cm/s to km/h
    }

    FString Info = FString::Printf(TEXT("Gear: %d\nRPM: %d\nSpeed: %d km/h"), Gear, RPM, Speed);
    DrawText(Info, FColor::White, 50, 50, nullptr, 2.0f, false);
}

void ASRTTHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    // Clean up widget when HUD is destroyed
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
}
