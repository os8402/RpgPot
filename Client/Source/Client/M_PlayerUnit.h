// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "M_PlayerUnit.generated.h"

UCLASS()
class CLIENT_API AM_PlayerUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AM_PlayerUnit();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void AttackCheck();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInteruppted);


private:

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* _Cam;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UM_Anim* _Anim;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _HpBar;


private:

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool _bAttacking;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	int32 _Hp;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	int32 _MaxHp;
};
