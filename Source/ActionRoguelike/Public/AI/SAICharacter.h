// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;
class UUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HeatlhBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	FName TimeToHitParamName;

	void SetTargetActor(AActor* TargetActor);

	UFUNCTION()
	void OnPawnSeen(APawn* pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	virtual void PostInitializeComponents() override;
	
	
	
};
