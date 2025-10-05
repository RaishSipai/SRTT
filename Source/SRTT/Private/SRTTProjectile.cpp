// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTTProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SRTTDamageInterface.h" // Include our damage interface

ASRTTProjectile::ASRTTProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Use a sphere as a simple collision representation.
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASRTTProjectile::OnHit);
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement.
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 20000.0f;
	ProjectileMovementComp->MaxSpeed = 20000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;
}

void ASRTTProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Destroy the projectile after 3 seconds.
	SetLifeSpan(3.0f);
}

void ASRTTProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only run damage logic on the server.
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		// Make sure we didn't hit ourselves or the actor that spawned us.
		if (OtherActor && OtherActor != this && OtherActor != GetOwner())
		{
			// --- DECOUPLED DAMAGE ---
			// Check if the actor we hit implements our damage interface.
			if (OtherActor->Implements<USRTTDamageInterface>())
			{
				// If it does, call the interface function.
				// This works for vehicles, destructible barrels, anything that implements the interface.
				ISRTTDamageInterface::Execute_ApplyVehicleDamage(OtherActor, 20.f, Hit, GetInstigatorController(), this);
			}
		}
	}

	// Destroy the projectile on impact.
	Destroy();
}
