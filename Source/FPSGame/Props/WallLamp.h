// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/Object_Interface.h"
#include "Engine/StaticMeshActor.h"
#include "WallLamp.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AWallLamp : public AStaticMeshActor, public IObject_Interface
{
	GENERATED_BODY()
	
public:
	AWallLamp();

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Wall Light")
	class USpotLightComponent* SpotLight;

	UFUNCTION(Reliable, Server, WithValidation)
	void TurnLamp();
	void TurnLamp_Implementation();
	bool TurnLamp_Validate();

	UFUNCTION()
	void OnRep_LampOff();

	UPROPERTY(ReplicatedUsing = OnRep_LampOff)
	bool bLampOff;
	
	UFUNCTION()
	void OnRep_LightColor();
	
	UPROPERTY(ReplicatedUsing = OnRep_LightColor)
	FLinearColor LightColor;

};