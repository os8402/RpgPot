// Fill out your copyright notice in the Description page of Project Settings.


#include "M_Controller.h"
#include "M_PlayerUnit.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>


AM_Controller::AM_Controller()
{
	bShowMouseCursor = true;

}

void AM_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Move", IE_Pressed, this, &AM_Controller::OnMovePressed);
}

void AM_Controller::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}


void AM_Controller::MoveToMouseCursor()
{
	_bMoveToMouseCursor = true;

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	_Target = Cast<ACharacter>(Hit.Actor);

	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.ImpactPoint);
	}
}

void AM_Controller::SetNewDestination(const FVector Dest)
{
	APawn* const MyPawn = GetPawn();

	if (MyPawn)
	{
		float const Distance = FVector::Dist(Dest, MyPawn->GetActorLocation());

		if ((Distance > 10.f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Dest);
		}

	}
}

void AM_Controller::OnMovePressed()
{
	MoveToMouseCursor();
}
