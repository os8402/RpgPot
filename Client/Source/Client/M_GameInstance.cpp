// Fill out your copyright notice in the Description page of Project Settings.


#include "M_GameInstance.h"

UM_GameInstance::UM_GameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("DataTable'/Game/Resources/Data/StatTable.StatTable'"));
	MyStats = DATA.Object;
}

void UM_GameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance %d"), GetStatData(1)->Attack);
}

FPlayerUnitData* UM_GameInstance::GetStatData(int32 Level)
{
	return MyStats->FindRow<FPlayerUnitData>(*FString::FromInt(Level), TEXT(""));
}
