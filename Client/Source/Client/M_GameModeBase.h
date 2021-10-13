// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "M_GameModeBase.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDied, ACharacter*, Character);


UCLASS()
class CLIENT_API AM_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AM_GameModeBase();

protected:
    virtual void BeginPlay() override;

    //Called when Player character has died.
    UFUNCTION()
    virtual void PlayerDied(ACharacter* Character);

    UPROPERTY()
    FOnPlayerDied OnPlayerDied;

  

public:
    const FOnPlayerDied& GetOnPlayerDied() const { return OnPlayerDied; }

    //Tries to Spawn the player's pawn.
    virtual void RestartPlayer(AController* NewPlayer) override;

    void RespawnMonster(const FVector& Pos);

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AActor> _Monster; 

    

};
