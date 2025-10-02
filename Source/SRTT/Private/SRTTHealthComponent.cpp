// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTHealthComponent.h"
#include "Net/UnrealNetwork.h" // Required for variable replication

// Sets default values for this component's properties
USRTTHealthComponent::USRTTHealthComponent()
{
	// Set this component to be initialized when the game starts
	PrimaryComponentTick.bCanEverTick = false;

	// Set default values
	MaxHealth = 100.0f;

	// Set this component to be replicated
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USRTTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// We only want the server to handle the damage logic.
	if (GetOwnerRole() == ROLE_Authority)
	{
		// Bind our damage handling function to the owner Actor's OnTakeAnyDamage event.
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USRTTHealthComponent::HandleTakeAnyDamage);
		}
	}

	// Set initial health
	Health = MaxHealth;
	
}


void USRTTHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	// Clamp the health to prevent it from going below 0 or above MaxHealth.
	const float OldHealth = Health;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	const float DamageApplied = OldHealth - Health;

	// Broadcast the OnHealthChanged event to any listening classes.
	if (DamageApplied > 0.f)
	{
		OnHealthChanged.Broadcast(this, Health, -DamageApplied, DamageType);
	}

	// If health is zero, broadcast the death event.
	if (Health <= 0.0f)
	{
		OnDeath.Broadcast();
	}
}

void USRTTHealthComponent::OnRep_Health(float OldHealth)
{
	const float DamageApplied = OldHealth - Health;

	// Broadcast the OnHealthChanged event on clients. This is how the client's UI gets updated.
	OnHealthChanged.Broadcast(this, Health, -DamageApplied, nullptr);
}

// Called every frame
void USRTTHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// This function tells the engine which variables to replicate from the server to clients.
void USRTTHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the 'Health' variable to all clients.
	DOREPLIFETIME(USRTTHealthComponent, Health);
}