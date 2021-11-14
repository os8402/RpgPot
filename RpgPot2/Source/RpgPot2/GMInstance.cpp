// Fill out your copyright notice in the Description page of Project Settings.


#include "GMInstance.h"
#include "UnitCharacter.h"

UGMInstance::UGMInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> StatData(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));

	if (StatData.Succeeded())
		_statData = StatData.Object;

	static ConstructorHelpers::FClassFinder<AUnitCharacter> EM(TEXT("Blueprint'/Game/Blueprints/BP_UnitCharacter.BP_UnitCharacter_C'"));

	if (EM.Succeeded())
		_spawnEnemy = EM.Class;
	
}

void UGMInstance::Init()
{
	Super::Init();

	GetWorld()->GetTimerManager().SetTimer(_respawnTimer, this,  &UGMInstance::RespawnEnemy, 4.f, true);

}

void UGMInstance::RespawnEnemy()
{
	if (_totalEnemyCount < 5)
	{
		FActorSpawnParameters spawnParams;

		int32 x = FMath::RandRange(-800, 800);
		int32 y = FMath::RandRange(-800, 800);
		FVector spawnLoc = FVector(x, y, 120.f);
		FRotator spawnRot;
		
		auto respawnEnemy = Cast<AUnitCharacter>(
			GetWorld()->SpawnActor<AActor>(_spawnEnemy, spawnLoc, spawnRot, spawnParams));

		if (respawnEnemy == nullptr)
			return;

		int32 key = _totalEnemyCount << 24;

		respawnEnemy->SetIndex(key);

		_enemyList.Add(key++,  respawnEnemy);

		_totalEnemyCount++;
	}

}

void UGMInstance::DestroyEnemy(int32 index)
{
	GetWorld()->DestroyActor(_enemyList[index]);
	_totalEnemyCount--;
}

FUnitData* UGMInstance::GetStatData(int32 level)
{
	return _statData->FindRow<FUnitData>(*FString::FromInt(level), TEXT(""));
}


