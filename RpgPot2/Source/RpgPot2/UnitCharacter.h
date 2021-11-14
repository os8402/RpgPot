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
	enum GameStates { IDLE, MOVE, ATTACK, DEAD };
	enum GameEvents { ON_ENTER, ON_UPDATE };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	GameStates _gameState = GameStates::IDLE;
	GameEvents _gameEvent = GameEvents::ON_ENTER;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	//idle
	void SearchActorInfo();


	void ChaseTheEnemy();
	void AttackEnemy();
	void AttackCheck();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void VisibleHpBar();
	void ChangeMinimapColor(FLinearColor color);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted);

public:

	void SetIndex(int32 index) { _index = index; }
	int32 GetIndex() { return _index; };

	void SetFSMState(GameStates newState);
	GameStates GetFSMState() { return _gameState; }

	USkeletalMeshComponent* GetOutLineMesh() { return _outLineMesh; }

	TArray<class UUnitAnim*>& GetUnitAnim() { return _animInsArr; }

	class UStatDataComponent* GetStatComp() { return _statComp; }

	bool IsAttacking() { return _bAttacking; }

	FOnAttackEnded& GetOnAttackEnded() { return _onAttackEnded; }
	TWeakObjectPtr<class AUnitCharacter>& GetEnemyTarget(){ return _enemyTarget;}
	void SetEnemyTarget(class AUnitCharacter* target) { _enemyTarget = target; }
	
private:



	//FSM 
	void FSMUpdate();

	void IdleEnter();
	void IdleUpdate();
	void IdleExit();

	void MoveEnter();
	void MoveUpdate();
	void MoveExit();

	void AttackEnter();
	void AttackUpdate();
	void AttackExit();

	void DeadEnter();
	void DeadUpdate();
	void DeadExit();

private:

	int32 _index = 0;

private:

	//기본 카메라 + 메쉬

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* _cam;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float _targetArmLength = 800.f;
	
	//0 : 기본 메시 , 1 : 아웃라인 메시. 
	UPROPERTY(VisibleAnywhere)
	TArray<class UUnitAnim*> _animInsArr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* _outLineMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UMaterial* _outLineMat;


private: 

	//공격 진행을 위한 타겟. 
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AUnitCharacter> _enemyTarget;

	bool _bAttacking = false;

	FOnAttackEnded _onAttackEnded;

	//죽음 처리
private:
	//일정시간 지나면 월드에서 제거. 
	FTimerHandle _deadHandle;


private:

	//UI
	UPROPERTY(VisibleAnyWhere, Category = "UI")
	TSubclassOf<class AActor> _dmgActor;

	UPROPERTY(VisibleAnyWhere, Category = "UI")
	class UWidgetComponent* _hpBar; 

	FTimerHandle _hpBarTimerHandle;

private:

	//스탯

	UPROPERTY(VisibleAnywhere,  Category = "Stat")
	class UStatDataComponent* _statComp;

	//미니맵
private:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _minimapSpring;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* _minimapCam;

	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* _minimapIcon; 

};
