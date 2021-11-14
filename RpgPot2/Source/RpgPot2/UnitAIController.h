// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	AUnitAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;


private:

	
	FTimerHandle _timerHandle;

	UPROPERTY()
	class UBehaviorTree* _behaviourTree;

	UPROPERTY()
	class UBlackboardData* _blackBoardData; 
};
