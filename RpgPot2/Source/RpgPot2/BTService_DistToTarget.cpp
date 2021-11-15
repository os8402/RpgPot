// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistToTarget.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "UnitAIController.h"
#include "UnitMonster.h"
#include "UnitPlayer.h"

UBTService_DistToTarget::UBTService_DistToTarget()
{
	NodeName = TEXT("DistToTarget");
	Interval = 1.f;
}


void UBTService_DistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto owned = Cast<AUnitMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (owned == nullptr || owned->GetFSMState() == AUnitCharacter::DEAD)
		return;

	auto target = Cast<AUnitPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (target == nullptr)
		return;

	float dist = owned->GetDistanceTo(target);

	if (dist > 1000.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}

}
