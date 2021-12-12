// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitMonster.h"
#include "UnitAIController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "GMInstance.h"
#include <kismet/GameplayStatics.h>
#include "StatDataComponent.h"
#include "DmgTextActor.h"


AUnitMonster::AUnitMonster()
{
	SetActorRelativeLocation(FVector(0.f, 0.f, 100.f));
}


float AUnitMonster::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float dmg = Super::TakeDamage(Damage , DamageEvent , EventInstigator, DamageCauser);

	FLinearColor color = FLinearColor::White;

	_currentDmgActor->UpdateDamage(dmg, color);

	return dmg;
}

void AUnitMonster::DeadCharacter()
{
	Super::DeadCharacter();

	//만약 죽었을 경우 플레이어 쪽에선 nullptr을 잡고 있기 때문에 구독취소 해야함
	if (GetEnemyTarget().IsValid())
	{
		GetEnemyTarget().Get()->GetStatComp()->GetOnUnitDied().Remove(_dieHandle);
		//->GetStatComp()->GetOnUnitDied().AddUObject(owned, &AUnitMonster::SetTargetEmpty);
	}
	

	//AI
	auto aiController = Cast<AUnitAIController>(GetController());

	if (aiController)
	{
		aiController->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		aiController->GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), GetActorLocation());

		GetWorld()->GetTimerManager().SetTimer(_deadHandle, FTimerDelegate::CreateLambda([&]()
			{
				auto gmInstance = Cast<UGMInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				gmInstance->DestroyEnemy(GetIndex());

			}), 3.f, false);

	}

	
}

void AUnitMonster::SetTargetEmpty()
{
	if (GetEnemyTarget().IsValid() == false)
		return; 

	GetEnemyTarget().Reset();

	auto aiController = Cast<AUnitAIController>(GetController());

	if(aiController)
		aiController->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
}
