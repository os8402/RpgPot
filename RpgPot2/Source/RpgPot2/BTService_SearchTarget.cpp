// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "UnitAIController.h"
#include "UnitCharacter.h"
#include <DrawDebugHelpers.h>
#include "StatDataComponent.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto owned = Cast<AUnitCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (owned == nullptr || owned->GetFSMState() == AUnitCharacter::DEAD)
		return;


	UWorld* world = owned->GetWorld();
	FVector center = owned->GetActorLocation();
	float seachRadius = 500.f;

	if (world == nullptr)
		return;

	TArray<FOverlapResult> overlapResult;
	FCollisionQueryParams queryParams(NAME_None, false, owned);

	bool bResult =  world->OverlapMultiByChannel(
		overlapResult,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(seachRadius),
		queryParams);

	if (bResult)
	{
		for (auto& overlap : overlapResult)
		{
			AUnitCharacter* myCharacter = Cast<AUnitCharacter>(overlap.GetActor());
			if (myCharacter && myCharacter->GetController()->IsPlayerController())
			{

				if (myCharacter->GetFSMState() == AUnitCharacter::DEAD)
					return;

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), myCharacter);
				owned->SetEnemyTarget(myCharacter);

				//Á×¾úÀ¸¸é Å¸°Ù ÇØÁ¦
				myCharacter->GetStatComp()->GetOnUnitDied().AddLambda([&]()
					{	
						if (owned == nullptr)
							return;
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
					});
				
				DrawDebugSphere(world, center, seachRadius, 16, FColor::Green, false, .2f);
				
				return;
			}
		}
	
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}

	DrawDebugSphere(world, center, seachRadius, 16, FColor::Red, false, .2f);

}