// Fill out your copyright notice in the Description page of Project Settings.


#include "M_AI.h"
#include <NavigationSystem.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>


AM_AI::AM_AI()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Resources/AI/BT_M_PlayerAI.BT_M_PlayerAI'"));

	if (BT.Succeeded())
	{
		_BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/Resources/AI/BB_M_PlayerUnit.BB_M_PlayerUnit'"));

	if (BD.Succeeded())
	{
		_BlackBoardData = BD.Object;
	}

}

void AM_AI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

//	GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AM_AI::RandomMove, 3.f , true);

	if (UseBlackboard(_BlackBoardData, Blackboard))
	{
		if (RunBehaviorTree(_BehaviorTree))
		{
			//TODO
		}
	}
}

void AM_AI::OnUnPossess()
{
	Super::OnUnPossess();

	//GetWorld()->GetTimerManager().ClearTimer(_TimerHandle);
}

void AM_AI::RandomMove()
{
	
	auto CurPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr)
		return;

	FNavLocation RandomLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
	}
}

