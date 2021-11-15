// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitCharacter.h"
#include "UnitMonster.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API AUnitMonster : public AUnitCharacter
{
	GENERATED_BODY()

public:

	AUnitMonster();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void DeadCharacter() override;

	//Ÿ���� ����� ���
	void SetTargetEmpty();

	FDelegateHandle& GetDieHandle() { return _dieHandle; }

private:

	FDelegateHandle _dieHandle; 
};
