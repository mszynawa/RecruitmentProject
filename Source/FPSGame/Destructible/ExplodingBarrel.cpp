// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingBarrel.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "Weapons/FPSGameProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "UObjectGlobals.h"
//#include "Physics/BarrelPhysicalMaterial.h"

AExplodingBarrel::AExplodingBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	//Creates the Destructible Component
	//DestructibleComponent = CreateDefaultSubobject<DestructibleComponent>(TEXT("Destructible"));

	//Attaches the Destructible Component to Root
	GetDestructibleComponent()->SetupAttachment(RootComponent);
	GetDestructibleComponent()->SetNotifyRigidBodyCollision(true);
	//RootComponent->StartAwake(false);

	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> DefaultMeshAsset(TEXT("/Game/Meshes/DM_Barrel"));
	if (DefaultMeshAsset.Succeeded())
	{
		//Sets the Mesh inside the Component from the Asset.
		GetDestructibleComponent()->SetDestructibleMesh(DefaultMeshAsset.Object);
	}

	//BarrelPhysMaterial = GetWorld()->SpawnActor(()

	FRotator Rotation;
	Rotation.Roll = 90.0f;
	Rotation.Pitch = 0.0f;
	Rotation.Yaw = 0.0f;
	RootComponent->SetRelativeRotation(Rotation);

	RootComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	//RootComponent = DestructibleComponent;
	MaxHealth = 3;
	bIsExploded = false;

	SetReplicates(true);
	bNetLoadOnClient = true;
}

void AExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();

	GetDestructibleComponent()->OnComponentHit.AddDynamic(this, &AExplodingBarrel::OnHealthChanged);

	CurrentHealth = MaxHealth;

	DefaultDamage = 5.f;
	DefaultImpulse = 5000.f;
}

void AExplodingBarrel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExplodingBarrel, CurrentHealth);
	DOREPLIFETIME(AExplodingBarrel, bIsExploded);
}

void AExplodingBarrel::OnHealthChanged(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsExploded)
	{
		return;
	}

	if (!bIsExploded && OtherActor->ActorHasTag(TEXT("Projectile")))
	{
		CurrentHealth = CurrentHealth - 1.f;
		if (CurrentHealth <= 0.f)
		{
			//Replicate
			AFPSGameProjectile* projectile = Cast< AFPSGameProjectile>(OtherActor);
			Destroy(DefaultDamage, Hit.Location, NormalImpulse, DefaultImpulse);

			//Server
			OnRep_Exploded();
		}
	}
}

bool AExplodingBarrel::Destroy_Validate(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	return true;
}

void AExplodingBarrel::Destroy_Implementation(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	//DestructibleComponent->ApplyDamage(Damage, HitLocation, ImpulseDir, Impulse);
	GetDestructibleComponent()->SetSimulatePhysics(true);
	//RootComponent->GetActorLocation()
	GetDestructibleComponent()->ApplyRadiusDamage(Damage, GetActorLocation(), 0.0f, DefaultImpulse, true);

	GetDestructibleComponent()->OnComponentHit.RemoveDynamic(this, &AExplodingBarrel::OnHealthChanged);
}

void AExplodingBarrel::OnRep_Exploded()
{
	SetIsExploded(!bIsExploded);
}

void AExplodingBarrel::SetIsExploded_Implementation(bool NewExploded)
{
	bIsExploded = !bIsExploded;
}

bool AExplodingBarrel::SetIsExploded_Validate(bool NewExploded)
{
	return true;
}