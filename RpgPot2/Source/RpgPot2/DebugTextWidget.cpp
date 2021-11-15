// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTextWidget.h"
#include "Components/TextBlock.h"

void UDebugTextWidget::BindText(FString str)
{
	_TB_SpawnIndex->SetText(FText::FromString(str));

}