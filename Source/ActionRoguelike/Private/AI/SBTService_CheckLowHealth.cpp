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
			bool bLowHealth = USAttributeComponent::IsActorLowHealth(AIPawn, LowHealthThreshold);

			BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
	

}

USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	LowHealthThreshold = 0.4f;
}
