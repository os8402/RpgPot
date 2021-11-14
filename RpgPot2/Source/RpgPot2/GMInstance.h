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

private:

	UPROPERTY()
	class UDataTable* _statData; 

	UPROPERTY(VisibleAnyWhere, Category = Pawn)
	TSubclassOf<class AUnitCharacter> _spawnEnemy;  
		
	int32 _enemeyCount = 0;

	UPROPERTY(VisibleAnyWhere, Category = Pawn)
	TMap<int32,  class AUnitCharacter*> _enemyList; 

	FTimerHandle _respawnTimer; 
};
