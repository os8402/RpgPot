// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnitAnim.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackHit)

UCLASS()
class RPGPOT2_API UUnitAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UUnitAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	void PlayAttackMontage();

	UFUNCTION()
	void AnimNotify_AttackHit();

public:
	
	FOnAttackHit& GetOnAttackHit() { return _onAttackHit; }

	void SetDead() { StopAllMontages(.1f); _bDead = true; }


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess=true));
	float _speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true));
	bool _bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	UAnimMontage* _attackMontage;


	FOnAttackHit _onAttackHit;
};
