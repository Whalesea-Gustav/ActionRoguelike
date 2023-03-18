// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_CreditCoin.h"

#include "SPlayerState.h"

void ASPowerup_CreditCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditAmount);
		HideAndCooldownPowerup();
	}
}

ASPowerup_CreditCoin::ASPowerup_CreditCoin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	CreditAmount = 10;
}
