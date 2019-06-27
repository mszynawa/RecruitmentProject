// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "ExplodingBarrel.generated.h"

/**
 *
 */
UCLASS()
class FPSGAME_API AExplodingBarrel : public ADestructibleActor
{
	GENERATED_BODY()

public:
	AExplodingBarrel();

	UPROPERTY(Replicated)
	float CurrentHealth;
	float MaxHealth;

	float DefaultDamage;
	float DefaultImpulse;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bIsExploded;

	UFUNCTION()
	void OnRep_Exploded();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destructible")
	//class UDestructibleComponent* DestructibleComp;

	UFUNCTION()
	void OnHealthChanged(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse); //(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);
	void Destroy_Implementation(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);
	bool Destroy_Validate(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);

	UFUNCTION(Reliable, Server, WithValidation)
	void SetIsExploded(bool NewExploded);
	void SetIsExploded_Implementation(bool NewExploded);
	bool SetIsExploded_Validate(bool NewExploded);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};