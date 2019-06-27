// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Object_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObject_Interface : public UInterface
{
	GENERATED_BODY()
};

class FPSGAME_API IObject_Interface
{
	GENERATED_BODY()

public:
	//int32 Level;
	//int32 Strength;

	//UFUNCTION(Category = "Interface")
		//virtual bool OnFocusReceivedByObject();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//UFUNCTION(Reliable, Server, WithValidation)
	//UFUNCTION(Reliable, Server, WithValidation)
	void InteractWithObject();
	virtual void InteractWithObject_Implementation();
	//bool InteractWithObject_Validate();
};
