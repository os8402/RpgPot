// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitPlayer.generated.h"

UCLASS()
class RPGPOT_API AUnitPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	void Attack();
	void AttackCheck();

	

	
private:

	UPROPERTY(VisibleAnyWhere)
	class USpringArmComponent* _springArm;
	UPROPERTY(VisibleAnyWhere)
	class UCameraComponent* _cam;
	UPROPERTY(VisibleAnywhere)
	class UMyAnimInstance* _animInstance;


};
