// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthMax = 100.f;
	Health = HealthMax;
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool USAttributeComponent::Kill(AActor* InstitagorActor)
{
	return ApplyHealthChange(InstitagorActor, -GetHealthMax());
}

bool USAttributeComponent::HealFull(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, GetHealthMax());
}

USAttributeComponent* USAttributeComponent::GetAttributeComponent(AActor* Actor)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));

	return AttributeComp;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(Actor);

	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	
	return false;														
}

bool USAttributeComponent::IsActorLowHealth(AActor* Actor, float threshold)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(Actor);

	if (AttributeComp)
	{
		return AttributeComp->Health < threshold * AttributeComp->HealthMax;
	}

	return false;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth()
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{

	if (Delta < 0.0f && !GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float OldHeath = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	
	float ActualDelta = Health - OldHeath;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKill(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0.0f;
}


// Called every frame
void USAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

