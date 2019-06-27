// Fill out your copyright notice in the Description page of Project Settings.


#include "WallLamp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AWallLamp::AWallLamp()
{
	//PrimaryActorTick.bCanEverTick = true;

	//Static Mesh Component
	//StaticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("StaticMesh Component"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshAsset(TEXT("/Game/Meshes/SM_Lamp"));
	if (DefaultMeshAsset.Succeeded())
	{
		//Sets the Mesh inside the Component from the Asset.
		GetStaticMeshComponent()->SetStaticMesh(DefaultMeshAsset.Object);
	}

	//Point Light
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Wall Light"));
	SpotLight->SetupAttachment(RootComponent);
	
	FVector Position;
	Position.X = 0.0f;
	Position.Y = 94.0f;
	Position.Z = 0.0f;
	SpotLight->SetRelativeLocation(Position);

	FRotator Rotation;
	Rotation.Roll = 180.0f;
	Rotation.Pitch = -90.0f;
	Rotation.Yaw = 180.0f;
	SpotLight->SetRelativeRotation(Rotation);

	SpotLight->SetAttenuationRadius(16300.0f);
	SpotLight->Intensity = 100000.0f;
	SpotLight->bVisible = true;
	SpotLight->InnerConeAngle = 10.0f;
	SpotLight->OuterConeAngle = 60.0f;

	SpotLight->SetLightColor(FLinearColor(255, 66, 0));
	
	//bReplicates = true;
	SetReplicates(true);
}

void AWallLamp::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWallLamp, bLampOff);
	DOREPLIFETIME(AWallLamp, LightColor);
}

void AWallLamp::OnRep_LampOff()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LAMPA OFF/ON"));
	//SpotLight->SetVisibility(!bLampOff);
	//SpotLight->bVisible = false;

	SpotLight->SetVisibility(!bLampOff);
}

void AWallLamp::OnRep_LightColor()
{
	SpotLight->SetLightColor(LightColor);
}

bool AWallLamp::TurnLamp_Validate()
{
	return true;
}

void AWallLamp::TurnLamp_Implementation()
{   
	bLampOff = !bLampOff;
	OnRep_LampOff();

	//if (Role == ROLE_Authority)
	if (HasAuthority())
	{
		if (bLampOff)
		{
			LightColor = FLinearColor::MakeRandomColor();
			OnRep_LightColor();
			//SpotLight->SetLightColor(FLinearColor::MakeRandomColor()); //(FMath::RandRange(0, 255), FMath::RandRange(0, 255), FMath::RandRange(0, 255)));
		}
	}
	
}