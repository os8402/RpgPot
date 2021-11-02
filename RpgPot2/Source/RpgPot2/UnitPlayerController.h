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
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;


public:

	void MoveToMouseCursor();
	void SetMoveDest(const FVector DestLocation);

	void CheckEnemy(ACharacter* Enemy, ACharacter* MyCharacter);

	void OnMovePressed();
	void OnMoveReleased();


private:
	int32 _bClickMouse = 0;

	UPROPERTY(EditAnywhere, Category = "Cursor")
	TSubclassOf<class UUserWidget> _cursorTest;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorBasic;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _cursorAttack;

};
