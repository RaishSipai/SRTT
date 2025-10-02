// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRTTBaseWeapon.generated.h"

class ASRTTProjectile;

UCLASS()
class SRTT_API ASRTTBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASRTTBaseWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> MuzzleLocation;

	// The class of projectile this weapon should fire. Set in Blueprint.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASRTTProjectile> ProjectileClass;

	// Shots per second.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	// The base damage of a single shot.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	// Timer handle for automatic fire.
	FTimerHandle TimerHandle_AutomaticFire;

	// Is the player currently holding down the fire button?
	bool bWantsToFire;

public:
	// Called by the WeaponSystemComponent.
	void StartFire();
	void StopFire();

protected:
	// The actual function that fires a single shot.
	void Fire();

	// Server RPC for firing. The client asks the server to fire.
	UFUNCTION(Server, Reliable)
	void ServerFire();
};
