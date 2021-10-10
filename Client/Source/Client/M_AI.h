// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "M_AI.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AM_AI : public AAIController
{
	GENERATED_BODY()

public:

	AM_AI();

	virtual void OnPossess(APawn* InPawn);
	virtual void OnUnPossess();

	void RandomMove();

private:

	FTimerHandle _TimerHandle;

	UPROPERTY()
	class UBehaviorTree* _BehaviorTree; 

	UPROPERTY()
	class UBlackboardData* _BlackBoardData;
	
};
