// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API UInGameMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void BindHp(class UStatDataComponent* statComp);
	void UpdateHp();


private:

	UPROPERTY(meta = (BindWidget))
	class UImage* _Img_InGameMain;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* _PB_HpGauge;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* _PB_MpGauge;

	TWeakObjectPtr<class UStatDataComponent> _currentStatComp;
	
};
