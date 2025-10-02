// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRTTWeaponSystemComponent.generated.h"

class ASRTTBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SRTT_API USRTTWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USRTTWeaponSystemComponent();

protected:
	virtual void BeginPlay() override;

	// This will hold the Blueprint classes of the weapons we want to equip by default.
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSystem")
	TArray<TSubclassOf<ASRTTBaseWeapon>> DefaultWeaponClasses;

	// An array to hold the actual weapon instances that are spawned at runtime.
	UPROPERTY(VisibleInstanceOnly, Category = "WeaponSystem")
	TArray<TObjectPtr<ASRTTBaseWeapon>> EquippedWeapons;

	// The index of the currently active weapon in the EquippedWeapons array.
	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "WeaponSystem")
	int32 CurrentWeaponIndex;

public:
	/** Called by the Pawn to start firing the current weapon. */
	void StartFire();

	/** Called by the Pawn to stop firing the current weapon. */
	void StopFire();
};
