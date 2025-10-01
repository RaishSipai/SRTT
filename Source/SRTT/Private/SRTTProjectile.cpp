// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "SRTTWheeledVehiclePawn.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"

// Sets default values
ASRTTProjectile::ASRTTProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create collision component
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(10.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASRTTProjectile::OnHit);
    RootComponent = CollisionComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    WeaponMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("WeaponMovementComponent"));
    WeaponMovementComponent->InitialSpeed = 1000.f;
    WeaponMovementComponent->MaxSpeed = 2000.f;
    WeaponMovementComponent->bRotationFollowsVelocity = true;
    WeaponMovementComponent->ProjectileGravityScale = 0.0f;

    Damage = 50.f;
    ProjectileLifetime = 5.0f; // 5 seconds lifetime
}

// Called when the game starts or when spawned
void ASRTTProjectile::BeginPlay()
{
	Super::BeginPlay();
	
    // Set timer to destroy projectile after lifetime
    GetWorldTimerManager().SetTimer(LifetimeTimer, this, &ASRTTProjectile::DestroyProjectile, ProjectileLifetime, false);
}

void ASRTTProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Don't hit self
    if (OtherActor == GetOwner())
        return;

    // Apply damage to hit actor
    if (OtherActor && OtherActor != this)
    {
        FPointDamageEvent DamageEvent;
        DamageEvent.Damage = Damage;
        DamageEvent.HitInfo = Hit;
        DamageEvent.ShotDirection = NormalImpulse;
        OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
    }

    // Destroy projectile on hit
    DestroyProjectile();
}

void ASRTTProjectile::DestroyProjectile()
{
    // Clear timer if it exists
    if (GetWorldTimerManager().IsTimerActive(LifetimeTimer))
    {
        GetWorldTimerManager().ClearTimer(LifetimeTimer);
    }

    // Stop movement component
    if (WeaponMovementComponent)
    {
        WeaponMovementComponent->StopMovementImmediately();
    }

    // Destroy the projectile
    Destroy();
}

void ASRTTProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Ensure timer is cleared when actor is destroyed
    if (GetWorldTimerManager().IsTimerActive(LifetimeTimer))
    {
        GetWorldTimerManager().ClearTimer(LifetimeTimer);
    }
    
    Super::EndPlay(EndPlayReason);
}

void ASRTTProjectile::InitProjectile(FVector InitialVelocity)
{
    if (WeaponMovementComponent)
    {
        WeaponMovementComponent->Velocity = InitialVelocity;
    }
}

