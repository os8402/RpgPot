// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPOT2_API UDebugTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindText(FString str);

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _TB_SpawnIndex;

	
};
