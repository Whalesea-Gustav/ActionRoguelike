// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_CreditCoin.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_CreditCoin : public ASPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Credits")
	int32 CreditAmount;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_CreditCoin();
};
