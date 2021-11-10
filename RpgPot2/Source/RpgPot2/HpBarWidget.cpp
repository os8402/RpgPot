// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarWidget.h"
#include "StatDataComponent.h"
#include "Components/ProgressBar.h"

void UHpBarWidget::BindHp(class UStatDataComponent* statComp)
{
	_currentStatComp = statComp;
	statComp->GetOnHpChanged().AddUObject(this, &UHpBarWidget::UpdateHp);
}

void UHpBarWidget::UpdateHp()
{
	if(_currentStatComp.IsValid())
		_PB_HpBar->SetPercent(_currentStatComp->GetHpRatio());
}
