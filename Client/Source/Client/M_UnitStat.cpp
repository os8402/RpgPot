// Fill out your copyright notice in the Description page of Project Settings.


#include "M_UnitStat.h"
#include "M_GameInstance.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UM_UnitStat::UM_UnitStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true; 

	_Level = 1;

	// ...
}


// Called when the game starts
void UM_UnitStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UM_UnitStat::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevel(_Level);

}

void UM_UnitStat::SetLevel(int32 NewLevel)
{
	auto CurGameInstance = Cast<UM_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (CurGameInstance)
	{
		auto StatData = CurGameInstance->GetStatData(NewLevel);
		if (StatData)
		{
			_Level = StatData->Level;
			SetHp(StatData->MaxHp);
			_MaxHp = StatData->MaxHp;
			_Attack = StatData->Attack;

		}
	}

}

void UM_UnitStat::SetHp(int32 NewHp)
{
	_Hp = NewHp;
	if (_Hp < 0)
		_Hp = 0;

	OnHpChanged.Broadcast();
}

void UM_UnitStat::OnAttacked(float DamageAmount)
{
	int32 NewHp = _Hp - DamageAmount;
	SetHp(NewHp);
}

