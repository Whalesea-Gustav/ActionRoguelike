// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LibMesh->SetRelativeRotation(FRotator(TargetPitch, 0.0f, 0.0f));
}

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LibMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LibMesh"));
	LibMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

