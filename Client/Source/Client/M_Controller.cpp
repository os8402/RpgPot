// Fill out your copyright notice in the Description page of Project Settings.


#include "M_Controller.h"
#include "M_PlayerUnit.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>


AM_Controller::AM_Controller()
{
	bShowMouseCursor = true;

}

void AM_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Move", IE_Pressed, this, &AM_Controller::OnMovePressed);
	InputComponent->BindAction("Move", IE_Released, this, &AM_Controller::OnMoveReleased);
}

void AM_Controller::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (_bMoveToMouseCursor)
	{
		MoveToMouseCursor(DeltaTime);
	}
}


void AM_Controller::MoveToMouseCursor(float DeltaTime)
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		_Target = Hit.Actor;
		SetNewDestination(Hit.ImpactPoint,  DeltaTime);	
	}
}



void AM_Controller::SetNewDestination(const FVector Dest,  float DeltaTime)
{
	
	ACharacter* MyCharacter = GetCharacter();

	FVector MyPos = MyCharacter->GetActorLocation();

	UCharacterMovementComponent* MovementComp = MyCharacter->GetCharacterMovement();
	float Speed = MovementComp->MaxWalkSpeed;
	FVector Direction = (Dest - MyPos).GetSafeNormal();
	MovementComp->MoveSmooth(Direction * Speed, DeltaTime);

	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyPos, Dest);
	FRotator SmoothRot = MyCharacter->GetActorRotation();

	TargetRot.Pitch = 0.f;
	TargetRot.Roll = 0.f;
	
	SmoothRot = FMath::RInterpTo(SmoothRot, TargetRot, DeltaTime, _InterpSpeed);
	MyCharacter->SetActorRotation(TargetRot);


}

void AM_Controller::OnMovePressed()
{
	_bMoveToMouseCursor = true;
}

void AM_Controller::OnMoveReleased()
{
	_bMoveToMouseCursor = false;
}
