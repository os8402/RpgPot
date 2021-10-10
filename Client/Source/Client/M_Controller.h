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


	void MoveToMouseCursor();
	void SetNewDestination(const FVector Dest);
	void OnMovePressed();
	
private: 
	bool _bMoveToMouseCursor = false; 
	
};
