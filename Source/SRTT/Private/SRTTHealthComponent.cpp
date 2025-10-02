// Copyright SRTT Studios. All Rights Reserved.

#include "SRTTHealthComponent.h"
#include "Net/UnrealNetwork.h"

USRTTHealthComponent::USRTTHealthComponent()
{
	// This component does not need to tick.
	PrimaryComponentTick.bCanEverTick = false;

	// Set default values.
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	// This is required for components to be replicated over the network.
	SetIsReplicatedByDefault(true);
}

void USRTTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only the server should have authority over health and set the initial value.
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		CurrentHealth = MaxHealth;
	}
}

void USRTTHealthComponent::OnRep_CurrentHealth()
{
	// This function is automatically called on clients when the 'CurrentHealth' variable changes.
	// We broadcast the OnHealthChanged event here so the UI can update.
	// We pass default/null values for Delta and Causer as they are only known on the server.
	OnHealthChanged.Broadcast(this, CurrentHealth, 0.f, nullptr);
}

void USRTTHealthComponent::HandleTakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	// Damage processing should ONLY happen on the server for security and authority.
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		return;
	}

	// Do not process damage if we have no health or the damage is zero/negative.
	if (DamageAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}

	// Apply the damage and clamp the health value to ensure it never goes below 0 or above MaxHealth.
	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	const float ActualDamage = OldHealth - CurrentHealth;

	// Broadcast the health changed event to any listening systems on the server.
	OnHealthChanged.Broadcast(this, CurrentHealth, -ActualDamage, DamageCauser);

	// If health has been depleted, broadcast the death event.
	if (CurrentHealth <= 0.0f)
	{
		OnDeath.Broadcast(EventInstigator, DamageCauser);
	}
}

void USRTTHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Tell the engine to replicate the CurrentHealth variable from the server to all clients.
	DOREPLIFETIME(USRTTHealthComponent, CurrentHealth);
}