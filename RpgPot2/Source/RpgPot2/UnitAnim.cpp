// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnim.h"

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

