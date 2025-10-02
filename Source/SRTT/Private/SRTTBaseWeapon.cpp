// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTTBaseWeapon.h"
#include "SRTTProjectile.h"
#include "GameFramework/Pawn.h"

ASRTTBaseWeapon::ASRTTBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	RootComponent = MuzzleLocation;

	FireRate = 10.0f;
	BaseDamage = 20.0f;
	bWantsToFire = false;
}

void ASRTTBaseWeapon::StartFire()
{
	bWantsToFire = true;
	// Calculate the time between shots.
	const float TimeBetweenShots = 1.0f / FireRate;
	// Start a looping timer to call the Fire() function.
	GetWorldTimerManager().SetTimer(TimerHandle_AutomaticFire, this, &ASRTTBaseWeapon::Fire, TimeBetweenShots, true, 0.0f);
}

void ASRTTBaseWeapon::StopFire()
{
	bWantsToFire = false;
	// Clear the timer to stop automatic fire.
	GetWorldTimerManager().ClearTimer(TimerHandle_AutomaticFire);
}

void ASRTTBaseWeapon::Fire()
{
	// On the client, we just call the Server RPC.
	// We also do a local check of bWantsToFire to stop the timer immediately if needed.
	if (GetLocalRole() < ENetRole::ROLE_Authority || !bWantsToFire)
	{
		if (!bWantsToFire)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_AutomaticFire);
		}
		ServerFire();
	}

	// On the server, we execute the firing logic directly.
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		ServerFire_Implementation();
	}
}

void ASRTTBaseWeapon::ServerFire_Implementation()
{
	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile.
		GetWorld()->SpawnActor<ASRTTProjectile>(ProjectileClass, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
	}
}
