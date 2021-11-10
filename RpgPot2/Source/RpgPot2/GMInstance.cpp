// Fill out your copyright notice in the Description page of Project Settings.


#include "GMInstance.h"

UGMInstance::UGMInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> StatData(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));

	if (StatData.Succeeded())
		_statData = StatData.Object;
}

void UGMInstance::Init()
{
	Super::Init();

}

FUnitData* UGMInstance::GetStatData(int32 level)
{
	return _statData->FindRow<FUnitData>(*FString::FromInt(level), TEXT(""));
}
