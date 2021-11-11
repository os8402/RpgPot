// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/WidgetComponent.h>
#include <Blueprint/UserWidget.h>
#include "UnitCharacter.h"
#include "UnitAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "InGameMainWidget.h"
#include "StatDataComponent.h"

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

	auto unitCharacter = Cast<AUnitCharacter>(GetPawn());
	if (unitCharacter)
	{
		//조종하는 캐릭터만 연두색
		unitCharacter->ChangeMinimapColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
	}

}

void AUnitPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	_owned = Cast<AUnitCharacter>(GetCharacter());

	if (_owned)
	{
		AUnitCharacter::GameStates state = _owned->GetFSMState();

		if (state == AUnitCharacter::DEAD)
			return;

		if (_bClickMouse)
		{
			MoveToMouseCursor();
		}

		if (state == AUnitCharacter::ATTACK)
		{
			ChaseEnemy();
		}

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

	if (_owned == nullptr)
		return;

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);

	if (Hit.bBlockingHit == false)
		return;

	auto hit = Cast<AUnitCharacter>(Hit.Actor);


	AUnitCharacter::GameStates state = _owned->GetFSMState();

	switch (state)
	{
	case AUnitCharacter::IDLE:

		//적을 타겟팅 하고 있었으면
		//공격 대상으로 변경 

		if (_currentSeeTarget.IsValid())
		{	
			if (hit && hit == _currentSeeTarget.Get())
			{
				_enemyTarget = _currentSeeTarget.Get();
				_enemyTarget.Get()->GetOutLineMesh()->SetVisibility(true);

				targetHandle = _enemyTarget.Get()->GetStatComp()->GetOnUnitDied()
					.AddUObject(this, &AUnitPlayerController::SetTargetEmpty);

				_owned->SetFSMState(AUnitCharacter::ATTACK);
				return;
			}
		}
	
		break;
	case AUnitCharacter::ATTACK:

		//TODO : 공격 취소 과정
		
		if (_enemyTarget.IsValid())
		{
			if (hit && hit == _enemyTarget.Get())
			{
				//여전히 같은 대상을 가리키고 있으므로... Return
				return;
			}
		}
		
		//취소 진행
		_owned->SetFSMState(AUnitCharacter::IDLE);

		_enemyTarget.Get()->GetStatComp()->GetOnUnitDied()
			.Remove(targetHandle);

		_enemyTarget.Get()->GetOutLineMesh()->SetVisibility(false);
		_enemyTarget.Reset();
		
		break;

	}

	for (auto animIns : _owned->GetUnitAnim())
	{
		animIns->StopAllMontages(.1f);
	}

	//전부 걸러졌으면 그땐 진짜 이동임. 
	SetMoveDest(Hit.ImpactPoint);


}

void AUnitPlayerController::SetMoveDest(const FVector DestLocation)
{

	if (_owned == nullptr)
		return;

	float const dist = FVector::Dist(DestLocation, _owned->GetActorLocation());

	if (dist > 100.f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}

}

void AUnitPlayerController::CheckActorOther(AUnitCharacter* other)
{
	if (_owned == nullptr)
		return;

	
	//타겟이 없거나 , 기존에 잡아둔 타겟과 다르고, 내가 아닐 때. 
	if (other && _owned != other)
	{

		//상대방이 죽었으면 탐지 x
		if (other->GetFSMState() == AUnitCharacter::DEAD)
			return;

		SetMouseCursorWidget(EMouseCursor::Default, _cursorAttack->GetUserWidgetObject());

		_currentSeeTarget = other;
		
		other->GetOutLineMesh()->SetVisibility(true);
	}	

	else
	{
		SetMouseCursorWidget(EMouseCursor::Default, _cursorBasic->GetUserWidgetObject());

		if (_currentSeeTarget.IsValid())
		{
			if (_enemyTarget.IsValid())
			{
				if (_currentSeeTarget.Get() == _enemyTarget.Get())
					return;
			}
			
			_currentSeeTarget.Get()->GetOutLineMesh()->SetVisibility(false);
			_currentSeeTarget.Reset();
		}

	}
	
}

void AUnitPlayerController::ChaseEnemy()
{

	if (_owned == nullptr)
		return;

	if (_enemyTarget.IsValid() == false)
		return;


	float const dist = FVector::Dist(
		_enemyTarget.Get()->GetActorLocation(),
		_owned->GetActorLocation());

	if (dist > 200.f)
	{
		SetMoveDest(_enemyTarget.Get()->GetActorLocation());
	}
	else
	{	
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, _owned->GetActorLocation());
		AttackEnemy(_owned);
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

void AUnitPlayerController::SetTargetEmpty()
{
	if (_enemyTarget.IsValid())
	{
		_enemyTarget.Reset();

		_owned->SetFSMState(AUnitCharacter::IDLE);

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

