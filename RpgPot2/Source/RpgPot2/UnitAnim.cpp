// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnim.h"

UUnitAnim::UUnitAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UM(TEXT("AnimMontage'/Game/Resources/Model/Katana/Animations/AM_AttackKatana.AM_AttackKatana'"));
	
	if (UM.Succeeded())
	{
		_attackMontage = UM.Object;
	}
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

