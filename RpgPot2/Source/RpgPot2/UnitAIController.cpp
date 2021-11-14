// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAIController.h"
#include <NavigationSystem.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>


AUnitAIController::AUnitAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/Resources/AI/BB_UnitCharacter.BB_UnitCharacter'"));

	if (BD.Succeeded())
	{
		_blackBoardData = BD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Resources/AI/BT_UnitCharacter.BT_UnitCharacter'"));

	if (BT.Succeeded())
	{
		_behaviourTree = BT.Object;
	}

}

void AUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AUnitAIController::RandomPatrol, 2.f, true);

	if (UseBlackboard(_blackBoardData, Blackboard))
	{
		if (RunBehaviorTree(_behaviourTree))
		{
		
		}

	}
}

void AUnitAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

