// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitGameModeBase.h"
#include "UnitCharacter.h"

AUnitGameModeBase::AUnitGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AUnitCharacter> AC(TEXT("Blueprint'/Game/Blueprints/BP_UnitCharacter.BP_UnitCharacter_C'"));
	if (AC.Succeeded())
	{
		DefaultPawnClass = AC.Class;
	}


}
