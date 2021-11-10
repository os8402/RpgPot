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

	void CheckEnemy(class AUnitCharacter* Enemy, class AUnitCharacter* MyCharacter);
	void ChaseEnemy();
	void AttackEnemy(AUnitCharacter* owned);

	void OnMovePressed();
	void OnMoveReleased();



public:
	TWeakObjectPtr<AUnitCharacter>& GetTarget() { return _enemyTarget; }


private:
	bool _bClickMouse = 0;

	UPROPERTY(EditAnywhere, Category = "Cursor")
	TSubclassOf<class UUserWidget> _cursorTest;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorBasic;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorAttack;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AUnitCharacter> _enemyTarget;

	bool _bAttacking = false; 

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UInGameMainWidget> _ingameMainClass;
	class UInGameMainWidget* _ingameMainUI;



};
