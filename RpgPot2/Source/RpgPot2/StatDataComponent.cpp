// Fill out your copyright notice in the Description page of Project Settings.


#include "StatDataComponent.h"
#include "GMInstance.h"
#include <kismet/GameplayStatics.h>


UStatDataComponent::UStatDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true; 
	// ...
	_level = 1;

}


void UStatDataComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatDataComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevel(_level);
}

void UStatDataComponent::SetLevel(int32 level)
{
	auto gmInstance = Cast<UGMInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (gmInstance)
	{
		auto statData = gmInstance->GetStatData(level);
		if (statData)
		{
			_level = statData->_level;
			SetHp(statData->_maxHp);
			_maxHp = statData->_maxHp;
			_attack = statData->_attack;
		}
	}

}

void UStatDataComponent::SetHp(int32 newHp)
{
	_hp = FMath::Max(0, newHp);

	if (_hp <= 0)
	{
		_onUnitDied.Broadcast();
	}

	_onHpChanged.Broadcast();
}

void UStatDataComponent::OnAttacked(int damageAmount)
{
	int32 newHp = _hp - damageAmount;
	SetHp(newHp);

}

