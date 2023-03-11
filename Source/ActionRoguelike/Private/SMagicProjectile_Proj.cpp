// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile_Proj.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
//#include "SGameplayInterface.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystem.h"


void ASMagicProjectile_Proj::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// USAttributeComponent* AttributeComp = Cast<USAttributeComponent> (OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		// if (AttributeComp)
		// {
		// 	AttributeComp->ApplyHealthChange(GetInstigator(), -1.0f * Damage);
		// 	
		// 	Destroy();
		// }

		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();
		}
	}
}

ASMagicProjectile_Proj::ASMagicProjectile_Proj()
{
	SphereComp->SetSphereRadius(20.f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile_Proj::OnActorOverlap);
}
