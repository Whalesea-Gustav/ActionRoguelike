// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		APawn* AIPawn = MyController->GetPawn();
		ACharacter* AICharacter = Cast<ACharacter>(AIPawn);
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>( MyController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - AICharacter->GetActorLocation();

		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		
		return NewActor == nullptr ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
