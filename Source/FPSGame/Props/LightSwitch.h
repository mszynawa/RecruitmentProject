// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/Object_Interface.h"
#include "Engine/StaticMeshActor.h"
#include "LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API ALightSwitch : public AStaticMeshActor, public IObject_Interface
{
	GENERATED_BODY()
	
public:
	ALightSwitch();

	UPROPERTY(VisibleAnywhere, Category = "Light Switch")
	class USphereComponent* LightSphere;

	//virtual void BeginPlay();

	//UFUNCTION()
	//void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Reliable, Server, WithValidation)
	void TurnLight();
	void TurnLight_Implementation();
	bool TurnLight_Validate();

	//void InteractWithObject();
	virtual void InteractWithObject_Implementation() override;
};
