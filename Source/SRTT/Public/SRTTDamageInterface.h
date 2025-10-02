// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SRTTDamageInterface.generated.h"

// This is the UObject wrapper for our interface, required by the engine for reflection.
// It should be marked as Blueprintable so Blueprints can also implement this interface.
UINTERFACE(MinimalAPI, Blueprintable)
class USRTTDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * The C++ interface contract for any actor that can be damaged in the game.
 * Any class that inherits from ISRTTDamageInterface guarantees it will have an ApplyVehicleDamage function.
 */
class SRTT_API ISRTTDamageInterface
{
	GENERATED_BODY()

public:

	// This is the function contract. Any class implementing this interface MUST provide its own version of this function.
	// UFUNCTION(BlueprintNativeEvent) allows this function to have a base C++ implementation AND be overridden in Blueprints.
	// UFUNCTION(BlueprintCallable) allows this function to be called from Blueprints.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void ApplyVehicleDamage(float DamageAmount, const FHitResult& HitResult, AController* EventInstigator, AActor* DamageCauser);
};

