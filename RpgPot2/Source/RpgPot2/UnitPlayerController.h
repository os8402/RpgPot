// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API AUnitPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AUnitPlayerController();
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:

	void MoveToMouseCursor();

	void SetMoveDest(const FVector DestLocation);

	void CheckActorOther(class AUnitCharacter* other);
	void ChaseEnemy();
	//void AttackEnemy(AUnitCharacter* owned);

	void PrimaryAttack_CameraShake();

	//Ÿ���� ����� ���
	void SetTargetEmpty();

	void OnMovePressed();
	void OnMoveReleased();



private:
	bool _bClickMouse = 0;
	bool _bAttacking = false; 


	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorBasic;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorAttack;

	//���� ���콺�� ���� �ִ� Ÿ�� ����
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AUnitCharacter> _currentSeeTarget;


	FDelegateHandle targetHandle;

	UPROPERTY(VisibleAnywhere)
	class AUnitCharacter* _owned; 

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UInGameMainWidget> _ingameMainClass;
	class UInGameMainWidget* _ingameMainUI;

	UPROPERTY()
	TSubclassOf<class UMatineeCameraShake> _CS_primaryAttack;




};
