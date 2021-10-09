// Fill out your copyright notice in the Description page of Project Settings.


#include "M_UnitWidget.h"
#include "M_UnitStat.h"
#include <Components/ProgressBar.h>

void UM_UnitWidget::BindHp(class UM_UnitStat* UnitStat)
{
	//PB_HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	CurUnitStat = UnitStat;
	UnitStat->OnHpChanged.AddUObject(this, &UM_UnitWidget::UpdateHp);
}

void UM_UnitWidget::UpdateHp()
{
	if(CurUnitStat.IsValid())
		PB_HpBar->SetPercent(CurUnitStat->GetHpRatio());
}
