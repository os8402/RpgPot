// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "M_GameInstance.generated.h"

/**
 * 
 */
USTRUCT()
struct FPlayerUnitData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;
};

UCLASS()
class CLIENT_API UM_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UM_GameInstance();
	virtual void Init() override;
	FPlayerUnitData* GetStatData(int32 Level);

private:

	UPROPERTY()
	class UDataTable* MyStats;

};
