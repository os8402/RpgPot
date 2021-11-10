// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API UHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void BindHp(class UStatDataComponent* statComp);
	void UpdateHp();

private:

	TWeakObjectPtr<class UStatDataComponent> _currentStatComp;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* _PB_HpBar;
	
};
