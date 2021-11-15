// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitGameModeBase.h"
#include "UnitPlayer.h"

AUnitGameModeBase::AUnitGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AUnitPlayer> AC(TEXT("Blueprint'/Game/Blueprints/Characters/BP_UnitPlayer.BP_UnitPlayer_C'"));
	if (AC.Succeeded())
	{
		DefaultPawnClass = AC.Class;
	}


}
