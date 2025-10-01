// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRTTProjectile.generated.h"

UCLASS()
class SRTT_API ASRTTProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRTTProjectile();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* WeaponMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ProjectileLifetime;

	UPROPERTY()
	FTimerHandle LifetimeTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called when projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Destroy projectile
	void DestroyProjectile();

public:	
	// Initialize projectile with a velocity vector
	void InitProjectile(FVector InitialVelocity);

};
