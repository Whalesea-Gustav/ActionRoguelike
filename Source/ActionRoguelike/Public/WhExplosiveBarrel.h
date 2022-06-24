// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WhExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AWhExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* RadialForceComp;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
		
	
public:	
	// Sets default values for this actor's properties
	AWhExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
