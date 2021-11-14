// Fill out your copyright notice in the Description page of Project Settings

#include "BTTask_FindPatrolPos.h"
#include "UnitAIController.h"
#include "UnitCharacter.h"
#include <NavigationSystem.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto owned =  Cast<AUnitCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (owned == nullptr)
		return EBTNodeResult::Failed;

	if (owned->GetFSMState() == AUnitCharacter::DEAD)
		return EBTNodeResult::Failed;


	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (navSystem == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation randLoc;

	if (navSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, randLoc))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), randLoc.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

}

