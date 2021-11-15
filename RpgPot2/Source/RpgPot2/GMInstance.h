// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GMInstance.generated.h"

USTRUCT()
struct FUnitData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere , BlueprintReadWrite)
	int32 _level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 _attack;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 _maxHp;

};

UCLASS()
class RPGPOT2_API UGMInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UGMInstance();

	virtual void Init() override;

	FUnitData* GetStatData(int32 level);

	void RespawnEnemy();
	void DestroyEnemy(int32 index);
	int32& GetTotalEnemyCount() { return _totalEnemyCount; }
	int32& GetKeyEnemyCount() { return _keyEnemyCount; }


	void RespawnPlayer(class AUnitPlayerController* controller);
	void DestroyPlayer(int32 index);

	int32 GenerateKey(int32 totalCount);
	void AddKeyPlayerCount(class AUnitPlayer* player);

	int32& GetTotalPlayerCount() { return _totalPlayerCount; }
	int32& GetKeyPlayerCount() { return _keyPlayerCount; }



private:

	UPROPERTY()
	class UDataTable* _statData; 

private:
	//PLAYER
	//몇명이 접속했는지 
	int32 _totalPlayerCount = 0;
	//얘는 계속 늘어나야 합니다.
	int32 _keyPlayerCount = 1;
	UPROPERTY(VisibleAnyWhere, Category = Pawn)
	TSubclassOf<class AUnitPlayer> _spawnPlayer;

	UPROPERTY(VisibleAnyWhere, Category = Pawn)
	TMap<int32, class AUnitPlayer*> _playerList;

private:
	//ENEMY
	int32 _totalEnemyCount = 0;
	//얘는 계속 늘어나야 합니다.
	int32 _keyEnemyCount = 1;

	UPROPERTY(VisibleAnyWhere, Category = Pawn)
	TSubclassOf<class AUnitMonster> _spawnEnemy;

	UPROPERTY(VisibleAnyWhere, Category = Pawn)
	TMap<int32,  class AUnitMonster*> _enemyList;

	FTimerHandle _respawnTimer; 
};
