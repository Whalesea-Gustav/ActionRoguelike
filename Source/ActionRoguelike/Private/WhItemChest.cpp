// Fill out your copyright notice in the Description page of Project Settings.


#include "WhItemChest.h"

#include "Engine/StaticMeshSocket.h"

void AWhItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

// Sets default values
AWhItemChest::AWhItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

}

// Called when the game starts or when spawned
void AWhItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWhItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

