// Fill out your copyright notice in the Description page of Project Settings.


#include "WhDashProjectile.h"

#include "Chaos/GeometryParticlesfwd.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

void AWhDashProjectile::OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ensureAlways(OtherComp); //trigger then stop in debug mode
	//check(OtherComp) //trigger then stop everything
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Hit : %s"), *(OtherComp->GetName())));
	Explode_TimeElasped();
}

AWhDashProjectile::AWhDashProjectile() : AWhProjectile()
{
	MovementComp->InitialSpeed = 3000.0f;

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComp");
	ParticleSystemComp->SetupAttachment(SphereComp);
	
}

void AWhDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	this->SphereComp->IgnoreActorWhenMoving(this->GetInstigator(), true);

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this,
		&AWhDashProjectile::Explode_TimeElasped, 0.2f);
}

void AWhDashProjectile::Explode_TimeElasped()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(),
		4.0f, 4, FColor::Green, false, 2.0f);
	ParticleSystemComp->Activate();
	MovementComp->StopMovementImmediately();

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this,
		&AWhDashProjectile::Dash_TimeElasped, 0.2f);
}

void AWhDashProjectile::Dash_TimeElasped()
{
	GetInstigator()->SetActorLocation(GetActorLocation());

	Destroy();
}

void AWhDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Regist Hit Handler Function ?
	SphereComp->OnComponentHit.AddDynamic(this, &AWhDashProjectile::OnCompHit);
}

void AWhDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}
