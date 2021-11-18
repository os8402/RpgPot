// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeadPanelWidget.generated.h"

/**
 * 
 */

UCLASS()
class RPGPOT2_API UDeadPanelWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;

	void InitUI(class AUnitPlayerController* pc);

	UFUNCTION()
	void RespawnBtnClicked();

	class UButton* GetRespawnBtn() { return  _BTN_Respawn; }

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _TB_DeadText;

	UPROPERTY(meta = (BindWidget))
	class UButton* _BTN_Respawn;

	UPROPERTY(meta = (BindWidget))
	class UButton* _BTN_Lobby;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* _fade;

	class AUnitPlayerController* _currentPC;
	
	
};
