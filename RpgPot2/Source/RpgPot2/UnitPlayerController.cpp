// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/WidgetComponent.h>
#include <Blueprint/UserWidget.h>
#include "UnitCharacter.h"
#include "UnitAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "InGameMainWidget.h"

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

	static ConstructorHelpers::FClassFinder<UInGameMainWidget> WB_Ingame(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WBP_InGameMain.WBP_InGameMain_C'"));
	if (WB_Ingame.Succeeded())
	{
		_ingameMainClass = WB_Ingame.Class;
	}
}

void AUnitPlayerController::BeginPlay()
{
	Super::BeginPlay();
	_ingameMainUI = CreateWidget<UInGameMainWidget>(this, _ingameMainClass);
	_ingameMainUI->AddToViewport();
}

void AUnitPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (_bClickMouse)
	{
		MoveToMouseCursor();
	}

	if (_bAttacking)
	{
		ChaseEnemy();
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

	_bAttacking = false;


	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);


	auto owned = Cast<AUnitCharacter>(GetCharacter());
	if (owned == nullptr)
		return;

	if (Hit.bBlockingHit)
	{
		if (_enemyTarget.IsValid())
		{
			auto Obj = Cast<AUnitCharacter>(Hit.Actor);
			
			if (Obj && Obj == _enemyTarget.Get())
			{
				_bAttacking = true;
				return;

			}
			_enemyTarget.Get()->GetOutLineMesh()->SetVisibility(false);
		}

		//_bAttacking = false;
		owned->GetUnitAnim()->StopAllMontages(.1f);
		_enemyTarget.Reset();

		SetMoveDest(Hit.ImpactPoint);
	}
}



void AUnitPlayerController::SetMoveDest(const FVector DestLocation)
{
	APawn* const myPawn = GetPawn();

	if (myPawn == nullptr)
		return;

	
	float const dist = FVector::Dist(DestLocation, myPawn->GetActorLocation());

	if (dist > 100.f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}

	
}

void AUnitPlayerController::CheckEnemy(AUnitCharacter* Enemy , AUnitCharacter* MyCharacter)
{

	if (Enemy && MyCharacter != Enemy)
	{
		SetMouseCursorWidget(EMouseCursor::Default, _cursorAttack->GetUserWidgetObject());
		_enemyTarget = Enemy;
		_enemyTarget.Get()->GetOutLineMesh()->SetVisibility(true);

	}	
	else
	{
		SetMouseCursorWidget(EMouseCursor::Default, _cursorBasic->GetUserWidgetObject());
		
		if (_bAttacking == false)
		{
			if (_enemyTarget.IsValid())
				_enemyTarget.Get()->GetOutLineMesh()->SetVisibility(false);

			_enemyTarget.Reset();
		}

	}
	
}

void AUnitPlayerController::ChaseEnemy()
{
	auto owned = Cast<AUnitCharacter>(GetCharacter());
	

	if (owned == nullptr)
		return;

	float const dist = FVector::Dist(
		_enemyTarget.Get()->GetActorLocation(),
		owned->GetActorLocation());

	if (dist > 200.f)
	{
		SetMoveDest(_enemyTarget.Get()->GetActorLocation());
	}
	else
	{	
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, owned->GetActorLocation());
		AttackEnemy(owned);
	}

	
}

void AUnitPlayerController::AttackEnemy(AUnitCharacter* owned)
{
	if (owned == nullptr || owned->IsAttacking())
		return;

	FVector ownedLoc = owned->GetActorLocation();
	FVector enemyLoc = FVector::ZeroVector;
	
	if (_enemyTarget.IsValid())
		enemyLoc = _enemyTarget.Get()->GetActorLocation();

	FRotator destRot = UKismetMathLibrary::FindLookAtRotation(ownedLoc, enemyLoc);

	owned->SetActorRotation(destRot);

	owned->Attack();

}

void AUnitPlayerController::OnMovePressed()
{
	_bClickMouse = true;
}

void AUnitPlayerController::OnMoveReleased()
{
	_bClickMouse = false;
}

