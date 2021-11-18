// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayerController.h"
#include <Components/WidgetComponent.h>
#include <Blueprint/UserWidget.h>
#include "UnitPlayer.h"
#include "UnitMonster.h"
#include "UnitAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include <kismet/GameplayStatics.h>
#include "InGameMainWidget.h"
#include "StatDataComponent.h"
#include <Camera/CameraShakeBase.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SceneCaptureComponent2D.h>
#include "InGameMainWidget.h"
#include "DeadPanelWidget.h"
#include "GMInstance.h"



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

	static ConstructorHelpers::FClassFinder<UDeadPanelWidget> WB_Dead(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WBP_DeadPanel.WBP_DeadPanel_C'"));
	if (WB_Dead.Succeeded())
	{
		_deadPanelClass = WB_Dead.Class;
	}

	static ConstructorHelpers::FClassFinder<UMatineeCameraShake> CS_PRIMARY_ATK(TEXT("Blueprint'/Game/Blueprints/Cameras/BP_CS_PrimaryAttack.BP_CS_PrimaryAttack_C'"));
	if (CS_PRIMARY_ATK.Succeeded())
	{
		_CS_primaryAttack = CS_PRIMARY_ATK.Class;
	}


}

void AUnitPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	_owned = Cast<AUnitPlayer>(GetCharacter());

	auto gmInstance = Cast<UGMInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	_owned->SetIndex(gmInstance->GenerateKey(gmInstance->GetKeyPlayerCount()));

	gmInstance->AddKeyPlayerCount(_owned);

	_owned->SetDebugText();

	InitPlayerUnit();

	UE_LOG(LogTemp, Log, TEXT("Index : %d") , _owned->GetIndex());
}

void AUnitPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AUnitPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void AUnitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	_ingameMainUI = CreateWidget<UInGameMainWidget>(this, _ingameMainClass);
	_ingameMainUI->AddToViewport();


	if (_ingameMainUI)
	{
		_ingameMainUI->BindHp(_owned->GetStatComp());
		_ingameMainUI->UpdateHp();
	}


}

void AUnitPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	_owned = Cast<AUnitPlayer>(GetCharacter());

	if (_owned)
	{
		AUnitPlayer::GameStates state = _owned->GetFSMState();

		if (state == AUnitPlayer::DEAD)
			return;

		if (_bClickMouse)	
			MoveToMouseCursor(DeltaTime);

		if (_bMoving)
			SetMoveDest(_destPos, DeltaTime);
		
		else if(_bAttacking)
			ChaseEnemy(DeltaTime);
	}

}

void AUnitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Move", IE_Pressed, this, &AUnitPlayerController::OnMovePressed);
	InputComponent->BindAction("Move", IE_Released, this, &AUnitPlayerController::OnMoveReleased);

}

void AUnitPlayerController::InitPlayerUnit()
{

	_bClickMouse = _bMoving = _bAttacking = false;

	auto unitCharacter = Cast<AUnitPlayer>(GetPawn());

	if (unitCharacter)
	{
		//�����ϴ� ĳ���͸� ���λ�
		unitCharacter->GetMinimapCam()->CaptureSortPriority = -1;
		unitCharacter->ChangeMinimapColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
		unitCharacter->GetStatComp()->SetHp(3000);
		unitCharacter->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	}


	if(_ingameMainUI)
	{
		_ingameMainUI->BindHp(_owned->GetStatComp());
		_ingameMainUI->UpdateHp();
	}

		

}

void AUnitPlayerController::MoveToMouseCursor(float deltaTime)
{
	_bClickMouse = true;
	_bAttacking = false;
	_bMoving = false;

	if (_owned == nullptr)
		return;

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);

	if (Hit.bBlockingHit == false)
		return;

	auto hit = Cast<AUnitMonster>(Hit.Actor);


	AUnitPlayer::GameStates state = _owned->GetFSMState();

	switch (state)
	{
	case AUnitPlayer::IDLE:

		//���� Ÿ���� �ϰ� �־�����
		//���� ������� ���� 

		if (_currentSeeTarget.IsValid())
		{	
			if (hit && hit == _currentSeeTarget.Get())
			{
				_owned->SetEnemyTarget(_currentSeeTarget.Get());
				_owned->GetEnemyTarget().Get()->GetOutLineMesh()->SetVisibility(true);

				targetHandle = _owned->GetEnemyTarget().Get()->GetStatComp()->GetOnUnitDied()
					.AddUObject(this, &AUnitPlayerController::SetTargetEmpty);

				_owned->SetFSMState(AUnitPlayer::ATTACK);
				_bAttacking = true; 
				return;
			}
		}
	
		break;
	case AUnitPlayer::ATTACK:

		//TODO : ���� ��� ����
		
		if (_owned->GetEnemyTarget().IsValid())
		{
			if (hit && hit == _owned->GetEnemyTarget().Get())
			{
				//������ ���� ����� ����Ű�� �����Ƿ�... Return
				_bAttacking = true;
				return;
			}
		}
		
		//��� ����
		_owned->SetFSMState(AUnitPlayer::IDLE);


		if (_owned->GetEnemyTarget().IsValid())
		{

			_owned->GetEnemyTarget().Get()->GetStatComp()->GetOnUnitDied()
				.Remove(targetHandle);

			_owned->GetEnemyTarget().Get()->GetOutLineMesh()->SetVisibility(false);
			_owned->GetEnemyTarget().Reset();

		}

		break;

	}

	for (auto animIns : _owned->GetUnitAnim())
	{
		animIns->StopAllMontages(.1f);
	}

	//���� �ɷ������� �׶� ��¥ �̵���. 
	_destPos = Hit.ImpactPoint;
	_bMoving = true; 
	

}

void AUnitPlayerController::SetMoveDest(const FVector DestLocation, float deltaTime)
{

	if (_owned == nullptr)
		return;

	float const dist = FVector::Dist(DestLocation, _owned->GetActorLocation());

	if (dist > 100.f)
	{
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);

		UCharacterMovementComponent* movementComp = _owned->GetCharacterMovement();
		float speed = movementComp->MaxWalkSpeed;
	
		FVector Dir = (DestLocation - _owned->GetActorLocation()).GetSafeNormal();
	
		movementComp->AddInputVector(Dir);
		
		FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(_owned->GetActorLocation(), DestLocation);
		targetRot.Pitch = 0.f;

		_smoothRot = FMath::RInterpTo(_smoothRot, targetRot, deltaTime, 10.f);
		
		_owned->SetActorRotation(_smoothRot);

		/*	UNavigationPath* navPath = UAIBlueprintHelperLibrary::GetCurrentPath(this);
			UAIBlueprintHelperLibrary::
			FNavPathSharedPtr ptr = navPath->GetPath();
			TArray<FNavPathPoint> dd = ptr->GetPathPoints();
			for (auto& a : dd)
				UE_LOG(LogTemp, Log, TEXT("Nav Location : %s"), *a.Location.ToString());*/

	}
	
}

void AUnitPlayerController::CheckActorOther(AUnitCharacter* other)
{
	if (_owned == nullptr)
		return;

	
	//Ÿ���� ���ų� , ������ ��Ƶ� Ÿ�ٰ� �ٸ���, ���� �ƴ� ��. 
	if (other && _owned != other)
	{

		//������ �׾����� Ž�� x
		if (other->GetFSMState() == AUnitPlayer::DEAD)
			return;

		SetMouseCursorWidget(EMouseCursor::Default, _cursorAttack->GetUserWidgetObject());

		if (_currentSeeTarget.IsValid())
		{
			_currentSeeTarget.Get()->GetOutLineMesh()->SetVisibility(false);
			_currentSeeTarget.Reset();
		}

		_currentSeeTarget = other;
		
		other->GetOutLineMesh()->SetVisibility(true);
	}	

	else
	{
		SetMouseCursorWidget(EMouseCursor::Default, _cursorBasic->GetUserWidgetObject());

		if (_currentSeeTarget.IsValid())
		{
			if (_owned->GetEnemyTarget().IsValid())
			{
				if (_currentSeeTarget.Get() == _owned->GetEnemyTarget().Get())
					return;
			}
			
			_currentSeeTarget.Get()->GetOutLineMesh()->SetVisibility(false);
			_currentSeeTarget.Reset();
		}

	}
	
}


void AUnitPlayerController::ChaseEnemy(float deltaTime)
{
	if (_owned->GetEnemyTarget().IsValid() == false)
		return;

	if (_owned->IsAttacking())
		return;

	float const dist = FVector::Dist(_owned->GetEnemyTarget().Get()->GetActorLocation(), _owned->GetActorLocation());

	if (dist > 200.f)
	{
		//SetMoveDest(_owned->GetEnemyTarget().Get()->GetActorLocation());
		//UAIBlueprintHelperLibrary::SimpleMoveToActor(this, _owned->GetEnemyTarget().Get());
		SetMoveDest(_owned->GetEnemyTarget().Get()->GetActorLocation(), deltaTime);


	}
	else
	{
		_owned->AttackEnemy();
	}
}

void AUnitPlayerController::PrimaryAttack_CameraShake()
{
	UCameraShakeBase* cam =  PlayerCameraManager->StartCameraShake(_CS_primaryAttack, 1.f);

}

void AUnitPlayerController::SetTargetEmpty()
{
	if (_owned->GetEnemyTarget().IsValid())
	{
		_owned->GetEnemyTarget().Reset();
		_owned->SetFSMState(AUnitPlayer::IDLE);

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

void AUnitPlayerController::OpenDeadPanel()
{
	_deadPanelUI = CreateWidget<UDeadPanelWidget>(this, _deadPanelClass);
	_deadPanelUI->AddToViewport();

	_deadPanelUI->InitUI(this);

}

