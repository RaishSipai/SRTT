// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISessionEventListenerInterface.generated.h"

UINTERFACE(Blueprintable)
class SRTT_API USRTTSessionEventListenerInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for listening to online session events.
 */
class SRTT_API ISRTTSessionEventListenerInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Session")
    void OnSessionCreated(bool bSuccess);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Session")
    void OnSessionJoined(bool bSuccess);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Session")
    void OnSessionDestroyed(bool bSuccess);
};

