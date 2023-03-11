// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

TAutoConsoleVariable<float> CVarImpulseMultiplier(
	TEXT("su.ImpulseMultiplier"),
	300000.f,
	TEXT("Impulse multiplier for ApplyDirectionalDamage"),
	ECVF_Cheat);

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageActor, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(TargetActor);
	if (AttributeComp)
	{
		AttributeComp->ApplyHealthChange(DamageActor, -DamageAmount);
		return true;
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageActor, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageActor, TargetActor, DamageAmount))
	{
		//Add Impulse to TargetActor
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * CVarImpulseMultiplier.GetValueOnGameThread(), HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
