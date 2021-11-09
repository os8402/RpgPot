// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DmgTextActor.generated.h"

UCLASS()
class RPGPOT2_API ADmgTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADmgTextActor();

protected:

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateDamage(int32 dmg);

private : 

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* _dmgText;

	float _curTime = 0.f;
	float _destroyTime = 1.f;


};
