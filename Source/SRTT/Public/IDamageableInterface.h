// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageableInterface.generated.h"

UINTERFACE(Blueprintable)
class SRTT_API USRTTDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for anything that can take or respond to damage.
 */
class SRTT_API ISRTTDamageableInterface
{
    GENERATED_BODY()

public:
    /** Apply incoming damage */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void ApplyDamage(float DamageAmount, AActor* DamageCauser);

    /** Returns true if this object is still alive */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    bool IsAlive() const;
};

