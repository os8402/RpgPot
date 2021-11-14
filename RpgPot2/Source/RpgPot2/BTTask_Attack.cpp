// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "UnitAIController.h"
#include "UnitCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; 
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto myCharacter = Cast<AUnitCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (myCharacter == nullptr)
		return EBTNodeResult::Failed;

	if (myCharacter->GetFSMState() == AUnitCharacter::DEAD)
		return EBTNodeResult::Failed;


	auto target = Cast<AUnitCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if(target == nullptr)
		return EBTNodeResult::Failed;

	myCharacter->SetEnemyTarget(target);
	myCharacter->SetFSMState(AUnitCharacter::ATTACK);

	_bAttacking = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bAttacking")), true);

	myCharacter->GetOnAttackEnded().AddLambda([this]()
		{
			_bAttacking = false;
		});

	return result;


}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (_bAttacking == false)
	{
		auto myCharacter = Cast<AUnitCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		myCharacter->SetFSMState(AUnitCharacter::MOVE);

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bAttacking")), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
		
}