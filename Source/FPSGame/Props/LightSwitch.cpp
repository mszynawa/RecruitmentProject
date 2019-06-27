// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitch.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WallLamp.h"
#include "Interface/Object_Interface.h"

ALightSwitch::ALightSwitch()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshAsset(TEXT("/Game/Meshes/SM_Cube"));
	if (DefaultMeshAsset.Succeeded())
	{
		//Sets the Mesh inside the Component from the Asset.
		GetStaticMeshComponent()->SetStaticMesh(DefaultMeshAsset.Object);
	}

	RootComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	//LightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LightSwitch Sphere"));
	//LightSphere->InitSphereRadius(300.0f);
	//LightSphere->SetCollisionProfileName(TEXT("Trigger"));
	//LightSphere->SetupAttachment(RootComponent);
}

/*
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();

	LightSphere->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitch::OnOverlapBegin);
	LightSphere->OnComponentEndOverlap.AddDynamic(this, &ALightSwitch::OnOverlapEnd);
}

void ALightSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		UE_LOG(LogTemp, Warning, TEXT("Activated"));

		TurnLight();
	}
}

void ALightSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Deactivated2"));
		TurnLight();
	}
}*/

void ALightSwitch::InteractWithObject_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Override object"));
	TurnLight();
}

void ALightSwitch::TurnLight_Implementation()
{
	if (Role == ROLE_Authority)
	{
		TArray<AActor*> WallLamps; // Rozwi¹zanie bardzo uproszczone - nie zd¹¿y³em wykonaæ tablicy z obiektami, które kontroluje dany LightSwitch.

		UGameplayStatics::GetAllActorsOfClass(this, AWallLamp::StaticClass(), WallLamps);

		for (int32 i = 0; i < WallLamps.Num(); i++)
		{
			AWallLamp* WallLamp = Cast<AWallLamp>(WallLamps[i]);
			if (WallLamp)
			{
				WallLamp->TurnLamp();
			}
		}
	}
}

bool ALightSwitch::TurnLight_Validate()
{
	return true;
}