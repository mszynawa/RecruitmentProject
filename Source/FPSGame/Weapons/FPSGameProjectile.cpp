// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FPSGameProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFPSGameProjectile::AFPSGameProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSGameProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	//RootComponent->Tags.Add(FName("Projectile");

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AFPSGameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		
		//AWall* destructibleWall(Cast<AWall>(OtherActor));
		//destructibleWall->DestructibleComp->ApplyDamage(10.0f, Hit.Location, NormalImpulse, 1.0f);
		//UE_LOG(LogTemp, Warning, TEXT("damage from hitComp: %s"), *HitComp->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("damage from otherActor: %s"), *OtherActor->GetName());

		//if (destructibleWall)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Hit destructible");
		//}

		if (Role == ROLE_Authority)
		{
			Destroy();
		}
	}
}