// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WhAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOneHealthChanged,AActor*, InstigatorActor, UWhAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UWhAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWhAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;


public:
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FOneHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);
	
};
