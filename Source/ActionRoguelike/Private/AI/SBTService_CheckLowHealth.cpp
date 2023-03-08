// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComponent))
	{
		AAIController* AIC = OwnerComp.GetAIOwner();

		APawn* AIPawn = AIC->GetPawn();
		
		if (ensure(AIPawn))
		{
			bool bLowHealth = USAttributeComponent::IsActorLowHealth(AIPawn, 0.5);

			BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
	

}
