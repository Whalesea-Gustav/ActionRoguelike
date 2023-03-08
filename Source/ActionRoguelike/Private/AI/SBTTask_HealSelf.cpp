// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"
#include "AIController.h"
#include "SAttributeComponent.h"


EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		APawn* AIPawn = MyController->GetPawn();

		AActor* AIActor = Cast<AActor>(AIPawn);

		if (AIActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponent(AIActor);

		if (AttributeComp == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		return AttributeComp->HealFull(AIActor) == false ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
