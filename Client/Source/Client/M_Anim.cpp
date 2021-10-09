// Fill out your copyright notice in the Description page of Project Settings.


#include "M_Anim.h"
#include <GameFramework/PawnMovementComponent.h>

UM_Anim::UM_Anim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UM(TEXT("AnimMontage'/Game/Resources/Animations/AM_M_PlayerUnit.AM_M_PlayerUnit'"));

	if (UM.Succeeded())
	{
		_AttackMontage = UM.Object;
	}
}

void UM_Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto CurPawn = TryGetPawnOwner();

	if (IsValid(CurPawn))
	{
		_Speed = CurPawn->GetVelocity().Size();
	}


}

void UM_Anim::PlayAttackMontage()
{
	Montage_Play(_AttackMontage, 1.f);
}

void UM_Anim::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}
