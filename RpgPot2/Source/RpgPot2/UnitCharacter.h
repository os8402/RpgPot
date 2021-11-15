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

	//FSM 
	enum  GameStates { IDLE, ATTACK, DEAD };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	GameStates _gameState = GameStates::IDLE;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	void AttackEnemy();
	virtual void AttackCheck();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void VisibleHpBar();
	void ChangeMinimapColor(FLinearColor color);
	virtual void DeadCharacter();


	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted);

public:

	void SetIndex(int32 index) { _index = index; }
	int32 GetIndex() { return _index; };

	//ĳ���� ���� �ε���
	void SetCharacterIndex(int32 index) { _chrIndex = index; }
	int32 GetCharacterIndex() { return _chrIndex; }

	void SetFSMState(GameStates newState);
	GameStates GetFSMState() { return _gameState; }

	USkeletalMeshComponent* GetOutLineMesh() { return _outLineMesh; }

	TArray<class UUnitAnim*>& GetUnitAnim() { return _animInsArr; }

	class UStatDataComponent* GetStatComp() { return _statComp; }

	bool IsAttacking() { return _bAttacking; }

	FOnAttackEnded _onAttackEnded;
	FOnAttackEnded& GetOnAttackEnded() { return _onAttackEnded; }
	TWeakObjectPtr<class AUnitCharacter>& GetEnemyTarget(){ return _enemyTarget;}
	void SetEnemyTarget(class AUnitCharacter* target) { _enemyTarget = target; }

	class USceneCaptureComponent2D* GetMinimapCam() { return _minimapCam; }

private:

	//FSM 
	//void FSMUpdate();	
	//void IdleUpdate();
	//void AttackUpdate();
	//void DeadUpdate();


private:

	//���� ���� ���� 
	int32 _index = 0;

	//ĳ���ͺ� ���� �ε���
	int32 _chrIndex = 0;

protected:

	//�⺻ ī�޶� + �޽�

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* _cam;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float _targetArmLength = 800.f;
	
	//0 : �⺻ �޽� , 1 : �ƿ����� �޽�. 
	UPROPERTY(VisibleAnywhere)
	TArray<class UUnitAnim*> _animInsArr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* _outLineMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UMaterial* _outLineMat;


protected: 

	//���� ������ ���� Ÿ��. 
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AUnitCharacter> _enemyTarget;

	bool _bAttacking = false;

	//���� ó��
protected:
	//�����ð� ������ ���忡�� ����. 
	FTimerHandle _deadHandle;


protected:

	//UI
	UPROPERTY(VisibleAnyWhere, Category = "UI")
	TSubclassOf<class AActor> _dmgActor;

	UPROPERTY(VisibleAnyWhere, Category = "UI")
	class UWidgetComponent* _hpBar; 

	FTimerHandle _hpBarTimerHandle;

protected:

	//����

	UPROPERTY(VisibleAnywhere,  Category = "Stat")
	class UStatDataComponent* _statComp;

	//�̴ϸ�
protected:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _minimapSpring;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* _minimapCam;

	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* _minimapIcon; 

};
