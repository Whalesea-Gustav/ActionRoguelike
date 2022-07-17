// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WhCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UWhInteractionComponent;
class UAnimMontage;
class UWhAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AWhCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf<AActor> DashProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;
	
	FTimerHandle TimerHandle_PrimaryAttack;

	
	
public:
	// Sets default values for this character's properties
	AWhCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
		UWhInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWhAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();

	void PrimaryAttack_TimeElasped();

	void BlackholeAttack();

	void BlackholeAttack_TimeElasped();
	
	void DashAttack();
	
	void DashAttack_TimeElasped();

	void PrimaryInteract();
	
	void Jump();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
