// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEnded)

UCLASS()
class RPGPOT2_API AUnitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitCharacter();

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

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void VisibleHpBar();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted);

public:
	
	USkeletalMeshComponent* GetOutLineMesh() { return _outLineMesh; }
	
	class UUnitAnim* GetUnitAnim() { return _animIns; }
	
	bool IsAttacking() { return _bAttacking; }
	
	FOnAttackEnded& GetOnAttackEnded() { return _onAttackEnded; }

private:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* _cam;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float _targetArmLength = 800.f;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* _outLineMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UMaterial* _outLineMat;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TArray<class UMaterial*> _originMats;

	UPROPERTY(VisibleAnywhere)
	class UUnitAnim* _animIns;

	bool _bAttacking = false;

	UPROPERTY(VisibleAnyWhere, Category = "UI")
	TSubclassOf<class AActor> _dmgActor;

	UPROPERTY(VisibleAnyWhere, Category = "UI")
	class UWidgetComponent* _hpBar; 

	FOnAttackEnded _onAttackEnded;

	FTimerHandle timerHandle;

	UPROPERTY(VisibleAnywhere)
	class UStatDataComponent* _statComp;

	//UPROPERTY(VisibleAnywhere)
	//class USpringArmComponent* _minimapSpring;

	//UPROPERTY(VisibleAnywhere)
	//class USceneCaptureComponent2D* _minimapCam;

	//UPROPERTY(VisibleAnywhere)
	//class UPaperSpriteComponent* _minimapIcon; 



};
