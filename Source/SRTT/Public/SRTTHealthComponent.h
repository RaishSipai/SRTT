// Copyright SRTT Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "SRTTHealthComponent.generated.h"

// The delegate that will be broadcast when health changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, USRTTHealthComponent*, OwningHealthComp, float, Health, float, HealthDelta, AActor*, DamageCauser);
// The delegate that will be broadcast when health reaches zero.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SRTT_API USRTTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USRTTHealthComponent();

	// --- DELEGATES ---
	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Events")
	FOnDeathSignature OnDeath;

protected:
	virtual void BeginPlay() override;

	// --- PROPERTIES ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	float MaxHealth;

	// This variable is replicated. When its value changes on the server, the OnRep_CurrentHealth function is called on all clients.
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, BlueprintReadOnly, Category = "HealthComponent")
	float CurrentHealth;

	// The function that will be called on clients when CurrentHealth is updated.
	UFUNCTION()
	void OnRep_CurrentHealth();

public:
	// The public function that will be called (via our interface) to apply damage.
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void HandleTakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);

	// Required function to set up property replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};