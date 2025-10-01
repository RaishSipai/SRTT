// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTPlayerState.h"
#include "Net/UnrealNetwork.h"

ASRTTPlayerState::ASRTTPlayerState()
{
	// Initialize health to maximum value
	Health = 100.f;
}

void ASRTTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASRTTPlayerState, Health);
}

void ASRTTPlayerState::SetHealth(float NewHealth)
{
	// Clamp health between 0 and 100
	Health = FMath::Clamp(NewHealth, 0.f, 100.f);
}

float ASRTTPlayerState::GetHealth() const
{
	return Health;
}
