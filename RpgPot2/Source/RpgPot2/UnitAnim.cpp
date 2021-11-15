// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnim.h"
#include "UnitCharacter.h"

UUnitAnim::UUnitAnim()
{

}

void UUnitAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (IsValid(pawn))
	{
		_speed = pawn->GetVelocity().Size();
	}

}

void UUnitAnim::PlayAttackMontage()
{
	Montage_Play(_attackMontage, 1.0f);
}

void UUnitAnim::AnimNotify_AttackHit()
{
	_onAttackHit.Broadcast();
}
