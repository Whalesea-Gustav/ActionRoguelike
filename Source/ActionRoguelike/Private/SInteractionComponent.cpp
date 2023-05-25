// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveLinearColor.h"
#include "SWorldUserWidget.h"

TAutoConsoleVariable<bool> CVarbbDebugDrawInteraction(TEXT("su.bInteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 500.f;
	collisionChannel = ECC_WorldDynamic;
	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	if(FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to Interact");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarbbDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(collisionChannel);
	
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
	End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	FHitResult Hit;

	//v1
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);
	AActor* HitActor = Hit.GetActor();

	//v2
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//Clear Before Try to Fill
	FocusedActor = nullptr;
	
	for (auto LocalHit : Hits)
	{
		bool bBreak = false;
		if (AActor* LocalHitActor = Hit.GetActor())
		{
			if (LocalHitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = LocalHitActor;
				bBreak = true;
			}
		}
		
		if (bDrawDebug) DrawDebugSphere(GetWorld(), LocalHit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		if (bBreak) break;
	}

	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if(DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
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

	FindBestInteractable();
	// ...
}

