// Fill out your copyright notice in the Description page of Project Settings.


#include "WhAttributeComponent.h"

// Sets default values for this component's properties
UWhAttributeComponent::UWhAttributeComponent()
{
	Health = 100;
}


bool UWhAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

