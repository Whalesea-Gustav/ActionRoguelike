// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WhProjectile.h"
#include "WhDashProjectile.generated.h"

class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API AWhDashProjectile : public AWhProjectile
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleSystemComp;
	
	FTimerHandle TimerHandle_Explode;

	FTimerHandle TimerHandle_Dash;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
		
	
public:
	AWhDashProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode_TimeElasped();

	void Dash_TimeElasped();

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
