// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SRTTPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SRTT_API ASRTTPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    ASRTTPlayerState();

    /** Player health (0�100) */
    UPROPERTY(Replicated, BlueprintReadWrite)
    float Health = 100.f;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Set health with validation (0-100 range) */
    UFUNCTION(BlueprintCallable, Category = "Player State")
    void SetHealth(float NewHealth);

    /** Get current health */
    UFUNCTION(BlueprintCallable, Category = "Player State")
    float GetHealth() const;
};
