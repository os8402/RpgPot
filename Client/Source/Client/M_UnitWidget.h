// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "M_UnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UM_UnitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHp(class UM_UnitStat* UnitStat);

	void UpdateHp();

private:
	TWeakObjectPtr<class UM_UnitStat> CurUnitStat;


	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_HpBar;
	
};
