// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponOwnerInterface.generated.h"

UINTERFACE(Blueprintable)
class SRTT_API USRTTWeaponOwnerInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for any actor that can own or control a weapon.
 */
class SRTT_API ISRTTWeaponOwnerInterface
{
    GENERATED_BODY()

public:
    /** Get the world-space location of the weapon's muzzle */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
    FVector GetMuzzleLocation() const;

    /** Get the world-space rotation of the weapon's muzzle */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
    FRotator GetMuzzleRotation() const;

    /** Get the controller responsible for firing */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
    AController* GetOwnerController() const;
};

