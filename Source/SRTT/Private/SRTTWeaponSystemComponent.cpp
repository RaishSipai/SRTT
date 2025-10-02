// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTTWeaponSystemComponent.h"
#include "SRTTBaseWeapon.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

USRTTWeaponSystemComponent::USRTTWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	CurrentWeaponIndex = 0;
}

void USRTTWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// Weapon spawning should only happen on the server.
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		// Spawn each of the default weapons.
		for (TSubclassOf<ASRTTBaseWeapon> WeaponClass : DefaultWeaponClasses)
		{
			if (WeaponClass)
			{
				ASRTTBaseWeapon* NewWeapon = GetWorld()->SpawnActor<ASRTTBaseWeapon>(WeaponClass, SpawnParams);
				EquippedWeapons.Add(NewWeapon);

				// TODO: Attach weapon to a specific socket on the vehicle mesh.
				// For now, we'll just attach to the root.
				NewWeapon->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			}
		}
	}
}

void USRTTWeaponSystemComponent::StartFire()
{
	// Check if we have a valid weapon at the current index.
	if (EquippedWeapons.IsValidIndex(CurrentWeaponIndex))
	{
		ASRTTBaseWeapon* CurrentWeapon = EquippedWeapons[CurrentWeaponIndex];
		if (CurrentWeapon)
		{
			// Tell the current weapon to start firing.
			CurrentWeapon->StartFire();
		}
	}
}

void USRTTWeaponSystemComponent::StopFire()
{
	if (EquippedWeapons.IsValidIndex(CurrentWeaponIndex))
	{
		ASRTTBaseWeapon* CurrentWeapon = EquippedWeapons[CurrentWeaponIndex];
		if (CurrentWeapon)
		{
			// Tell the current weapon to stop firing.
			CurrentWeapon->StopFire();
		}
	}
}

void USRTTWeaponSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USRTTWeaponSystemComponent, CurrentWeaponIndex);
}
