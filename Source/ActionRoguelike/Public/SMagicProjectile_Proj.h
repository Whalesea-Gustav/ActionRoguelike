// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SMagicProjectile_Proj.generated.h"

class UParticleEmitter;
class UParticleSystem;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile_Proj : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASMagicProjectile_Proj();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
	
	UFUNCTION(BlueprintCallable)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);
	
};
