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