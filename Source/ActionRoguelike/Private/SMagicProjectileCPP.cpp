// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectileCPP.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystem.h"

void ASMagicProjectileCPP::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}


void ASMagicProjectileCPP::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent> (OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-50.f);
			Destroy();
		}
	}
}

ASMagicProjectileCPP::ASMagicProjectileCPP() : ASProjectileBase()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectileCPP::OnActorOverlap);
}
