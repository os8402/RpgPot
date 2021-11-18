// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadPanelWidget.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include "GMInstance.h"
#include <kismet/GameplayStatics.h>
#include "UnitPlayerController.h"
#include "UnitPlayer.h"

void UDeadPanelWidget::NativeConstruct()
{

	_TB_DeadText->SetText(FText::AsNumber(19));

	PlayAnimation(_fade);

	if (_BTN_Respawn)
	{
		_BTN_Respawn->OnClicked.AddDynamic(this, &UDeadPanelWidget::RespawnBtnClicked);
	}
}

void UDeadPanelWidget::InitUI(class AUnitPlayerController* pc)
{
	_currentPC = pc;
}

void UDeadPanelWidget::RespawnBtnClicked()
{
	auto gmInstance = Cast<UGMInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	auto deadPlayer = Cast<AUnitPlayer>(_currentPC->GetPawn());

	gmInstance->RespawnPlayer(_currentPC);

	gmInstance->DestroyPlayer(deadPlayer->GetIndex());

	RemoveFromViewport();

}
