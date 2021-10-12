// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "M_Anim.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

UCLASS()
class CLIENT_API UM_Anim : public UAnimInstance
{
	GENERATED_BODY()

public:

	UM_Anim();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage(bool bPlaying = true);
	FName GetAttckMontageName(int32 SectionIndex);
	void JumpToSection(int32 SectionIndex);

	void PlayDeadAnim();



private:

	UFUNCTION()
	void AnimNotify_AttackHit();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _AttackMontage;
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float _Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool _bDead;

public:
	FOnAttackHit OnAttackHit;

};
