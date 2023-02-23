// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectileCPP.generated.h"

class UParticleEmitter;
class UParticleSystem;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectileCPP : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	
public:

	ASMagicProjectileCPP();
	
	UFUNCTION(BlueprintCallable)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);
	
};
