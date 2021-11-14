// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMainWidget.h"
#include "StatDataComponent.h"
#include "Components/ProgressBar.h"

void UInGameMainWidget::BindHp(class UStatDataComponent* statComp)
{
	_currentStatComp = statComp;
	statComp->GetOnHpChanged().AddUObject(this, &UInGameMainWidget::UpdateHp);
}

void UInGameMainWidget::UpdateHp()
{
	if (_currentStatComp.IsValid())
		_PB_HpGauge->SetPercent(_currentStatComp->GetHpRatio());
}
