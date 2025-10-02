// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRTTHealthComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE macros create a broadcast event that other classes can subscribe to.
// This is how we avoid hard dependencies. The Health Component doesn't know about the UI, it just broadcasts an event.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, USRTTHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SRTT_API USRTTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRTTHealthComponent();

	// The event broadcasted when health changes. The UI will bind to this.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	// The event broadcasted when health reaches zero. The vehicle pawn will bind to this.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathSignature OnDeath;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// The current health of the actor. Replicated so all clients know the value.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "HealthComponent")
	float Health;

	// The maximum health of the actor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	float MaxHealth;

	// Function bound to the owner Actor's TakeDamage event.
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// This function is called on clients whenever the 'Health' variable is updated from the server.
	UFUNCTION()
	void OnRep_Health(float OldHealth);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Returns the current health.
	UFUNCTION(BlueprintPure, Category = "HealthComponent")
	float GetHealth() const { return Health; }
		
};
