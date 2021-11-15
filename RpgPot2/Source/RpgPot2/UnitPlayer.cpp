// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayer.h"
#include "UnitPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "GMInstance.h"
#include <kismet/GameplayStatics.h>


AUnitPlayer::AUnitPlayer()
{


}

// Called every frame
void AUnitPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchActorInfo();

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
	if (playerController)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, GetActorLocation());
	}
	
	//playerController->OnUnPossess();
	
	auto gmInstance = Cast<UGMInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gmInstance->RespawnPlayer(playerController);

	gmInstance->DestroyPlayer(GetIndex());

}
