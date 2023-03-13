// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile_Proj.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
//#include "SGameplayInterface.h"
#include "SActionComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"


void ASMagicProjectile_Proj::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{

		//static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("State.Parrying"));
		
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -1.f * MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
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
