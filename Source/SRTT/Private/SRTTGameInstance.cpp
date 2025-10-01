// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTGameInstance.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"

void USRTTGameInstance::Init()
{
    Super::Init();
    
    UE_LOG(LogTemp, Log, TEXT("SRTTGameInstance initialized."));
    
    // Initialize online subsystem
    if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
    {
        UE_LOG(LogTemp, Log, TEXT("Online Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Online Subsystem found"));
    }
    
    // Load game settings
    LoadGameSettings();
}

void USRTTGameInstance::LoadGameSettings()
{
    // TODO: Load game settings from config files
    UE_LOG(LogTemp, Log, TEXT("Loading game settings..."));
}
