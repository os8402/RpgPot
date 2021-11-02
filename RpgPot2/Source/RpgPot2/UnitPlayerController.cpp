// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/WidgetComponent.h>
#include <Blueprint/UserWidget.h>


AUnitPlayerController::AUnitPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;


	_cursorBasic = CreateDefaultSubobject<UWidgetComponent>(TEXT("CURSOR_BASIC"));
	static ConstructorHelpers::FClassFinder<UUserWidget> CB(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WBP_CursorBasic.WBP_CursorBasic_C'"));
	if (CB.Succeeded())
	{
		_cursorBasic->SetWidgetClass(CB.Class);
	}

	_cursorAttack = CreateDefaultSubobject<UWidgetComponent>(TEXT("CURSOR_ATTACK"));
	static ConstructorHelpers::FClassFinder<UUserWidget> CA(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WBP_CursorAttack.WBP_CursorAttack_C'"));
	if (CA.Succeeded())
	{
		_cursorAttack->SetWidgetClass(CA.Class);
	}


}

void AUnitPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (_bClickMouse)
	{
		MoveToMouseCursor();
	}
}

void AUnitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Move", IE_Pressed, this, &AUnitPlayerController::OnMovePressed);
	InputComponent->BindAction("Move", IE_Released, this, &AUnitPlayerController::OnMoveReleased);
}

void AUnitPlayerController::MoveToMouseCursor()
{
	_bClickMouse = true;

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);


	if (Hit.bBlockingHit)
	{
		SetMoveDest(Hit.ImpactPoint);
	}
}

void AUnitPlayerController::SetMoveDest(const FVector DestLocation)
{
	APawn* const myPawn = GetPawn();

	if (myPawn)
	{
		float const dist = FVector::Dist(DestLocation, myPawn->GetActorLocation());

		if (dist > 120.f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}

	}
}

void AUnitPlayerController::CheckEnemy(ACharacter* Enemy , ACharacter* MyCharacter )
{

	//auto uw = Cast<UUserWidget>(_cursorTest->StaticClass());

	if (Enemy)
	{
		SetMouseCursorWidget(EMouseCursor::Default, _cursorAttack->GetUserWidgetObject());
	}	
	else
	{
		SetMouseCursorWidget(EMouseCursor::Default, _cursorBasic->GetUserWidgetObject());
	}
	


}

void AUnitPlayerController::OnMovePressed()
{
	_bClickMouse = true;
}

void AUnitPlayerController::OnMoveReleased()
{
	_bClickMouse = false;
}
