// Fill out your copyright notice in the Description page of Project Settings.


#include "WhMagicProjectile.h"

#include "WhAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWhMagicProjectile::AWhMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWhMagicProjectile::OnActorOverlap);
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void AWhMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		UWhAttributeComponent* AttributeComp =  Cast<UWhAttributeComponent>(OtherActor->GetComponentByClass(UWhAttributeComponent::StaticClass()));

		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AWhMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWhMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

