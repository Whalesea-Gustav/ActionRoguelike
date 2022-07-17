// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WhProjectile.h"
#include "WhTPProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AWhTPProjectile : public AWhProjectile
{
	GENERATED_BODY()

public:
	AWhTPProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
