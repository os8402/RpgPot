// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "M_AI.h"
#include "M_PlayerUnit.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	auto CurrentOwner = Cast<AM_PlayerUnit>(OwnerComp.GetAIOwner()->GetPawn());

	if (CurrentOwner == nullptr)
		return;

	UWorld* World = CurrentOwner->GetWorld();
	FVector Center = CurrentOwner->GetActorLocation();
	float SearchRadius = 500.f;

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentOwner);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams
	);

	if (bResult)
	{

		for (auto& OverlapResult : OverlapResults)
		{
			AM_PlayerUnit* MyPlayerUnit = Cast<AM_PlayerUnit>(OverlapResult.GetActor());
			if (MyPlayerUnit && MyPlayerUnit->GetController()->IsPlayerController())
			{
				CurrentOwner->SetTarget(MyPlayerUnit);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), MyPlayerUnit);

				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);

				return;
			}
		}
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}


}
