// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitCharacter.h"
#include "UnitPlayer.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API AUnitPlayer : public AUnitCharacter
{
	GENERATED_BODY()
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	AUnitPlayer();
	//idle
	void SearchActorInfo();

	virtual void AttackCheck() override;

	virtual void DeadCharacter() override;
	

};
