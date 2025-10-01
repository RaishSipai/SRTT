// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTGameModeBase.h"
#include "SRTTPlayerController.h"
#include "SRTTPlayerState.h"
#include "SRTTGameStateBase.h"
#include "SRTTWheeledVehiclePawn.h"
#include "SRTTHUD.h"

ASRTTGameModeBase::ASRTTGameModeBase()
{
    PlayerControllerClass = ASRTTPlayerController::StaticClass();
    PlayerStateClass = ASRTTPlayerState::StaticClass();
    GameStateClass = ASRTTGameStateBase::StaticClass();
    DefaultPawnClass = ASRTTWheeledVehiclePawn::StaticClass();
    HUDClass = ASRTTHUD::StaticClass();
}
