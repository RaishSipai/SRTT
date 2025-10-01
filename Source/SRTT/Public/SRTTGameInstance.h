// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SRTTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SRTT_API USRTTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
private:
	/** Load game settings from config files */
	void LoadGameSettings();
};
