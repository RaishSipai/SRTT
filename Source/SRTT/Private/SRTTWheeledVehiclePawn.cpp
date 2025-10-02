// Copyright SRTT Studios. All Rights Reserved.

#include "SRTTWheeledVehiclePawn.h"
#include "SRTTHealthComponent.h"
#include "SRTTGearboxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

ASRTTWheeledVehiclePawn::ASRTTWheeledVehiclePawn()
{
	// Create all the components that make up our vehicle.
	HealthComponent = CreateDefaultSubobject<USRTTHealthComponent>(TEXT("HealthComponent"));
	GearboxComponent = CreateDefaultSubobject<USRTTGearboxComponent>(TEXT("GearboxComponent"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASRTTWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	// Bind our OnDeath function to the delegate in the HealthComponent.
	// This will now compile because the function signatures match.
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ASRTTWheeledVehiclePawn::OnDeath);
	}
}

// --- Input Delegation Functions ---

void ASRTTWheeledVehiclePawn::SetThrottleInput(float Value)
{
	if (auto* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		MoveComp->SetThrottleInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::SetSteeringInput(float Value)
{
	if (auto* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		MoveComp->SetSteeringInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::SetBrakeInput(float Value)
{
	if (auto* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		MoveComp->SetBrakeInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::SetHandbrakeInput(float Value)
{
	if (auto* MoveComp = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
	{
		// Convert the float input to a boolean before passing it to the physics component.
		MoveComp->SetHandbrakeInput(Value > 0.1f);
	}
}

void ASRTTWheeledVehiclePawn::HandleClutchInput(float Value)
{
	if (GearboxComponent)
	{
		GearboxComponent->HandleClutchInput(Value);
	}
}

void ASRTTWheeledVehiclePawn::HandleShiftUp()
{
	if (GearboxComponent)
	{
		GearboxComponent->HandleShiftUp();
	}
}

void ASRTTWheeledVehiclePawn::HandleShiftDown()
{
	if (GearboxComponent)
	{
		GearboxComponent->HandleShiftDown();
	}
}

// --- Interface Implementation ---
void ASRTTWheeledVehiclePawn::ApplyVehicleDamage_Implementation(float DamageAmount, const FHitResult& HitResult, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		// Delegate the damage handling to our HealthComponent using the correct function name.
		HealthComponent->HandleTakeDamage(DamageAmount, EventInstigator, DamageCauser);
	}
}

// --- Event Handler ---
void ASRTTWheeledVehiclePawn::OnDeath(AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("%s was destroyed by %s!"), *GetName(), *GetNameSafe(DamageCauser));

	// TODO: Implement proper vehicle destruction (VFX, sound, disable input).

	// For now, we will just destroy the actor.
	Destroy();
}