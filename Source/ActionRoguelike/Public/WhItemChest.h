// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WhGamePlayInterface.h"
#include "WhItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AWhItemChest : public AActor, public IWhGamePlayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	// Because Macros : UFUNCTION(BlueprintNativeEvent)
	virtual void Interact_Implementation(APawn* InstigatorPawn) override; 

protected:

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* LidMesh;

public:	
	// Sets default values for this actor's properties
	AWhItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
