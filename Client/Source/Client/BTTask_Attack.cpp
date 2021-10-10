// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "M_AI.h"
#include "M_PlayerUnit.h"
#include <BehaviorTree/BehaviorTreeComponent.h>

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; 
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurCharacter = Cast<AM_PlayerUnit>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurCharacter == nullptr)
		return EBTNodeResult::Failed;

	CurCharacter->Attack();
	_bIsAttacking = true; 

	CurCharacter->OnAttackEnd.AddLambda([this]()
		{
			_bIsAttacking = false; 
		});

	return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (_bIsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
