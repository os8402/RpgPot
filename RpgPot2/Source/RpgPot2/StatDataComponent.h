// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatDataComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpChanged)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPOT2_API UStatDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	

	void SetLevel(int32 level);
	void SetHp(int32 newHp);
	void OnAttacked(int damageAmount);

	int32 GetHp() { return _hp; }
	int32 GetMaxHp() { return _maxHp; }
	float GetHpRatio() { return  _hp / (float)_maxHp; }
	int32 GetLevel() { return _level; }
	int32 GetAttack() { return _attack; }
	FOnHpChanged& GetOnHpChanged() { return _onHpChanged; }

private:

	UPROPERTY(EditAnyWhere, Category="Stat" , meta=(AllowPrivateAccess=true))
	int32 _level;

	UPROPERTY(EditAnyWhere, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 _hp;
	
	UPROPERTY(EditAnyWhere, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 _maxHp;

	UPROPERTY(EditAnyWhere, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 _attack;

	FOnHpChanged _onHpChanged;

};
