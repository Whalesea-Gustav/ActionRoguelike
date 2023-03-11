// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveLinearColor.h"

TAutoConsoleVariable<bool> CVarbbDebugDrawInteraction(TEXT("su.bInteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDrawDebug = CVarbbDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* MyOwner = GetOwner();

	FVector Start, End;
	FVector EyeLocation;
	FRotator EyeRotation;
	
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	TSubclassOf<UCameraComponent> CameraClass = UCameraComponent::StaticClass();
	UCameraComponent* CameraComponent = Cast<UCameraComponent>(MyOwner->GetComponentByClass(CameraClass));
	if (CameraComponent)
	{
		//FVector CameraLocation = CameraComponent->GetComponentLocation();
		FVector CameraTargetLocation = CameraComponent->GetComponentLocation() + (CameraComponent->GetComponentRotation().Vector() * 1000.0f);

		//Bad Option: Use Camera Rotation to Do Interact
		//EyeRotation = (CameraTargetLocation - EyeLocation).Rotation();

		//EyeLocation = CameraLocation;
		//EyeRotation = CameraComponent->GetComponentRotation();
	}
	else
	{
		
	}

	Start = EyeLocation;
	End = EyeLocation + (EyeRotation.Vector() * 1000.0f);
	
	FHitResult Hit;

	//v1
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);
	AActor* HitActor = Hit.GetActor();

	//v2
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = 32.f;
	Shape.SetSphere(Radius);
	
	bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (auto LocalHit : Hits)
	{
		bool bBreak = false;
		if (AActor* LocalHitActor = Hit.GetActor())
		{
			if (LocalHitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(LocalHitActor, MyPawn);
				bBreak = true;
			}
		}
		
		if (bDrawDebug) DrawDebugSphere(GetWorld(), LocalHit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		if (bBreak) break;
	}
	
	if (bDrawDebug) DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0.0f, 2.0f);
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

