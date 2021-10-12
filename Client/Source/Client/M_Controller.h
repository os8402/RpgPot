// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "M_Controller.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AM_Controller : public APlayerController
{
	GENERATED_BODY()

public:

	AM_Controller();
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	void MoveToMouseCursor(float DeltaTime);
	void SetNewDestination(const FVector Dest, float DeltaTime);
	void OnMovePressed();
	void OnMoveReleased();

	
private: 

	bool _bMoveToMouseCursor = false; 
	
	float _InterpSpeed = 0.1f;
	TWeakObjectPtr<class AActor> _Target;  


	
};
