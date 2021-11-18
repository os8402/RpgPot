// Fill out your copyright notice in the Description page of Project Settings.


#include "GMInstance.h"
#include "UnitCharacter.h"
#include "UnitMonster.h"
#include "UnitPlayer.h"
#include "UnitPlayerController.h"

UGMInstance::UGMInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> StatData(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));

	if (StatData.Succeeded())
		_statData = StatData.Object;

	static ConstructorHelpers::FClassFinder<AUnitMonster> EM(TEXT("Blueprint'/Game/Blueprints/Characters/BP_UnitMonster.BP_UnitMonster_C'"));

	if (EM.Succeeded())
		_spawnEnemy = EM.Class;

	static ConstructorHelpers::FClassFinder<AUnitPlayer> EP(TEXT("Blueprint'/Game/Blueprints/Characters/BP_UnitPlayer.BP_UnitPlayer_C'"));

	if (EP.Succeeded())
		_spawnPlayer = EP.Class;

	
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
		_totalEnemyCount++;
		FActorSpawnParameters spawnParams;

		int32 x = FMath::RandRange(-800, 800);
		int32 y = FMath::RandRange(-800, 800);
		FVector spawnLoc = FVector(x, y, 120.f);
		FRotator spawnRot = FRotator(FQuat::Identity);
		
		auto respawnEnemy = Cast<AUnitMonster>(
			GetWorld()->SpawnActor<AActor>(_spawnEnemy, spawnLoc, spawnRot, spawnParams));

		if (respawnEnemy == nullptr)
			return;

		int32 key = GenerateKey(_keyEnemyCount);

		respawnEnemy->SetIndex(key);

		_enemyList.Add(_keyEnemyCount,  respawnEnemy);

		_keyEnemyCount++;

	}

}

void UGMInstance::DestroyEnemy(int32 index)
{
	int key = index >> 24;
	GetWorld()->DestroyActor(_enemyList[key]);
	_totalEnemyCount--;
}

void UGMInstance::RespawnPlayer(class AUnitPlayerController* controller)
{
	_totalPlayerCount++;
	FActorSpawnParameters spawnParams;

	int32 x = FMath::RandRange(-800, 800);
	int32 y = FMath::RandRange(-800, 800);
	FVector spawnLoc = FVector(x, y, 120.f);
	FRotator spawnRot = FRotator(FQuat::Identity);

	auto respawnPlayer = Cast<AUnitPlayer>(
		GetWorld()->SpawnActor<AActor>(_spawnPlayer, spawnLoc, spawnRot, spawnParams));
	
	if (controller)
		controller->OnPossess(respawnPlayer);

	AddKeyPlayerCount(respawnPlayer);
}

void UGMInstance::DestroyPlayer(int32 index)
{
	int key = index >> 24;
	GetWorld()->DestroyActor(_playerList[key]);
	_totalPlayerCount--;
}

int UGMInstance::GenerateKey(int32 totalCount)
{
	return  totalCount << 24;
}

void UGMInstance::AddKeyPlayerCount(class AUnitPlayer* player)
{
	_playerList.Add(_keyPlayerCount, player);

	_keyPlayerCount++;
}

FUnitData* UGMInstance::GetStatData(int32 level)
{
	return _statData->FindRow<FUnitData>(*FString::FromInt(level), TEXT(""));
}


