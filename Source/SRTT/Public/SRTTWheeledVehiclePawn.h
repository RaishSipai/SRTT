// Copyright SRTT Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SRTTDamageInterface.h" 
#include "SRTTWheeledVehiclePawn.generated.h"

class USRTTHealthComponent;
class USRTTGearboxComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SRTT_API ASRTTWheeledVehiclePawn : public AWheeledVehiclePawn, public ISRTTDamageInterface
{
	GENERATED_BODY()

public:
	ASRTTWheeledVehiclePawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USRTTHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USRTTGearboxComponent> GearboxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

public:
	// --- Public API for PlayerController ---
	void SetThrottleInput(float Value);
	void SetSteeringInput(float Value);
	void SetBrakeInput(float Value);
	void SetHandbrakeInput(float Value);
	void HandleClutchInput(float Value);
	void HandleShiftUp();
	void HandleShiftDown();

	// --- Interface Implementation ---
	// This function must be public to correctly override the interface.
	virtual void ApplyVehicleDamage_Implementation(float DamageAmount, const FHitResult& HitResult, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// --- Event Handler ---
	UFUNCTION()
	void OnDeath(AController* InstigatedBy, AActor* DamageCauser);
};