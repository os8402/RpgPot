// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarWidget.h"
#include "StatDataComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHpBarWidget::BindHp(FText name,  class UStatDataComponent* statComp)
{
	_currentStatComp = statComp;
	statComp->GetOnHpChanged().AddUObject(this, &UHpBarWidget::UpdateHp);
	_TB_NameLv->SetText(name);
}

void UHpBarWidget::UpdateHp()
{
	if(_currentStatComp.IsValid())
		_PB_HpBar->SetPercent(_currentStatComp->GetHpRatio());
}

