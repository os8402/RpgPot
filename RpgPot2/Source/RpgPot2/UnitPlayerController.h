// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnitPlayerController.generated.h"


UCLASS()
class RPGPOT2_API AUnitPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AUnitPlayerController();
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;


public:

	void InitPlayerUnit();

	void MoveToMouseCursor(float deltaTime);

	void SetMoveDest(const FVector DestLocation , float deltaTime);

	void CheckActorOther(class AUnitCharacter* other);
	void ChaseEnemy(float deltaTime);


	void PrimaryAttack_CameraShake();

	//타겟이 사라질 경우
	void SetTargetEmpty();
	void OnMovePressed();
	void OnMoveReleased();

	void OpenDeadPanel();

	class UInGameMainWidget* GetInGameWidget() { return _ingameMainUI; }
	class UDeadPanelWidget* GetDeadPanelWidget() { return _deadPanelUI; }


private:
	bool _bClickMouse = 0;
	bool _bMoving = false; 
	bool _bAttacking = false; 

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorBasic;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorAttack;

	//현재 마우스로 보고 있는 타겟 정보
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<class AUnitCharacter> _currentSeeTarget;


	FDelegateHandle targetHandle;

	UPROPERTY(VisibleAnywhere)
	class AUnitPlayer* _owned; 

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UInGameMainWidget> _ingameMainClass;
	class UInGameMainWidget* _ingameMainUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UDeadPanelWidget> _deadPanelClass;
	class UDeadPanelWidget* _deadPanelUI;

	UPROPERTY()
	TSubclassOf<class UMatineeCameraShake> _CS_primaryAttack;

	FVector _destPos;
	FRotator _smoothRot;
};
