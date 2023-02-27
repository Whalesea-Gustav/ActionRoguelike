// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

// class UCameraShake;
class UParticleSystem;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USound;
class USoundCue;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ImpactSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
