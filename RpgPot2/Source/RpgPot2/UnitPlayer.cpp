// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayer.h"
#include "UnitPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "GMInstance.h"
#include <kismet/GameplayStatics.h>
#include "InGameMainWidget.h"
#include "DmgTextActor.h"

AUnitPlayer::AUnitPlayer()
{
	SetActorRelativeLocation(FVector(0.f, 0.f, 100.f));

}

// Called every frame
void AUnitPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchActorInfo();

	
}

float AUnitPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float dmg = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	FLinearColor color = FLinearColor::Red;

	_currentDmgActor->UpdateDamage(dmg, color);

	return dmg;
}

void AUnitPlayer::SearchActorInfo()
{
	auto PC = Cast<AUnitPlayerController>(GetController());

	if (PC)
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Pawn, true, HitResult);

		if (HitResult.bBlockingHit)
		{
			auto Obj = Cast<AUnitCharacter>(HitResult.Actor);
			PC->CheckActorOther(Obj);
		}
	}
}

void AUnitPlayer::AttackCheck()
{
	Super::AttackCheck();

	//플레이어
	auto controller = Cast<AUnitPlayerController>(GetController());

	if (controller == nullptr)
		return;

	controller->PrimaryAttack_CameraShake();

}



void AUnitPlayer::DeadCharacter()
{
	Super::DeadCharacter();

	auto playerController = Cast<AUnitPlayerController>(GetController());
	playerController->OpenDeadPanel();


}
