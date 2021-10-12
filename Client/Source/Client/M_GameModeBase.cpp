// Fill out your copyright notice in the Description page of Project Settings.


#include "M_GameModeBase.h"
#include "M_Controller.h"
#include "M_PlayerUnit.h"

AM_GameModeBase::AM_GameModeBase()
{
	static ConstructorHelpers::FClassFinder<AM_PlayerUnit> MP(TEXT("Blueprint'/Game/Resources/Player/BP_M_PlayerUnit.BP_M_PlayerUnit_C'"));

	if (MP.Succeeded())
	{
		DefaultPawnClass = MP.Class;

	}

	static ConstructorHelpers::FClassFinder<AM_Controller> AM(TEXT("Class'/Script/Client.M_Controller'"));


	if (AM.Succeeded())
	{
		PlayerControllerClass = AM.Class;
	}

}

void AM_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Bind our Player died delegate to the Gamemode's PlayerDied function.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AM_GameModeBase::PlayerDied);
	}
}

void AM_GameModeBase::PlayerDied(ACharacter* Character)
{
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}

void AM_GameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}
