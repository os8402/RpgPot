// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/WidgetComponent.h>
#include <Components/SceneCaptureComponent2D.h>
#include "PaperSpriteComponent.h"
#include "UnitPlayerController.h"
#include "UnitAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "DmgTextActor.h"
#include "HpBarWidget.h"
#include "StatDataComponent.h"
#include "GMInstance.h"
#include <kismet/GameplayStatics.h>
#include "UnitAIController.h"
#include <BehaviorTree/BlackboardComponent.h>


// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING ARM"));
	_springArm->SetupAttachment(GetCapsuleComponent());
	_springArm->SetUsingAbsoluteRotation(true);
	_springArm->TargetArmLength = _targetArmLength;
	_springArm->SetRelativeRotation(FRotator(-60.f, 45.f, 0.f));
	_springArm->bDoCollisionTest = false;

	_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAM"));
	_cam->SetupAttachment(_springArm);
	_cam->bUsePawnControlRotation = false; 
	

	_outLineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OUTLINE"));
	_outLineMesh->SetupAttachment(GetCapsuleComponent());
	_outLineMesh->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UBlueprint> DT(TEXT("Blueprint'/Game/Blueprints/BP_FloatText.BP_FloatText'"));

	if (DT.Object)
	{
		_dmgActor = Cast<UClass>(DT.Object->GeneratedClass);
	}

	_hpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP_BAR"));
	_hpBar->SetupAttachment(GetMesh());
	_hpBar->SetWidgetSpace(EWidgetSpace::Screen);
	_hpBar->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_HpBar(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WBP_HpBar.WBP_HpBar_C'"));

	if (WBP_HpBar.Succeeded())
	{
		_hpBar->SetWidgetClass(WBP_HpBar.Class);
		_hpBar->SetDrawSize(FVector2D(300.f, 25.f));
	}

	_statComp = CreateDefaultSubobject<UStatDataComponent>(TEXT("STAT"));

	_minimapSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("MINIMAP_SPRING"));
	_minimapSpring->SetupAttachment(GetCapsuleComponent());
	_minimapSpring->SetUsingAbsoluteRotation(true);
	_minimapSpring->TargetArmLength = 300.f;
	_minimapSpring->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	_minimapSpring->bDoCollisionTest = false;

	_minimapCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MINIMAP_CAM"));
	_minimapCam->SetupAttachment(_minimapSpring);

	_minimapIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MINIMAP_ICON"));
	_minimapIcon->SetupAttachment(GetCapsuleComponent());

	AIControllerClass = AUnitAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUnitAnim* _originAnimIns = Cast<UUnitAnim>(GetMesh()->GetAnimInstance());
	UUnitAnim* _outLineAnimIns = Cast<UUnitAnim>(_outLineMesh->GetAnimInstance());

	_animInsArr.Add(_originAnimIns);
	_animInsArr.Add(_outLineAnimIns);


	for (auto& animIns : _animInsArr)
	{
		if (animIns)
		{
			animIns->OnMontageEnded.AddDynamic(this, &AUnitCharacter::OnAttackMontageEnded);
			
			//�ƿ����� �޽ô� �������� ����
			if(_outLineAnimIns != animIns)
				animIns->GetOnAttackHit().AddUObject(this, &AUnitCharacter::AttackCheck);
			//bind
			_statComp->GetOnUnitDied().AddUObject(animIns, &UUnitAnim::SetDead);

		}
	}

	_statComp->GetOnUnitDied().AddUObject(this, &AUnitCharacter::SetFSMState , GameStates::DEAD);

	_hpBar->InitWidget();
	_hpBar->SetVisibility(false);

	auto hpBarWidget = Cast<UHpBarWidget>(_hpBar->GetUserWidgetObject());
	if (hpBarWidget)
		hpBarWidget->BindHp(_statComp);

	//ó���� �������� ����
	ChangeMinimapColor(FLinearColor(1.f, 0.f, 0.f, 1.f));

}

// Called every frame
void AUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchActorInfo();

	FSMUpdate();

}

void AUnitCharacter::SearchActorInfo()
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


void AUnitCharacter::ChaseTheEnemy()
{
	
	if (_enemyTarget.IsValid() == false)
		return;

	auto playerController = Cast<AUnitPlayerController>(GetController());

	if (playerController == nullptr)
		return;


	float const dist = FVector::Dist(_enemyTarget.Get()->GetActorLocation(), GetActorLocation());

	if (dist > 200.f)
	{
		playerController->SetMoveDest(_enemyTarget.Get()->GetActorLocation());
	}
	else
	{
		_gameState = GameStates::ATTACK;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, GetActorLocation());
		
	}

}

void AUnitCharacter::AttackEnemy()
{
	if (_bAttacking)
		return;

	_bAttacking = true;

	FVector ownedLoc = GetActorLocation();
	FVector enemyLoc = FVector::ZeroVector;

	if (_enemyTarget.IsValid() == false)
	{
		_gameState = GameStates::IDLE;
		return;
	}
	
	enemyLoc = _enemyTarget.Get()->GetActorLocation();

	FRotator destRot = UKismetMathLibrary::FindLookAtRotation(ownedLoc, enemyLoc);

	SetActorRotation(destRot);

	for (auto animIns : GetUnitAnim())
	{
		animIns->PlayAttackMontage();
	}


	
}

void AUnitCharacter::AttackCheck()
{

	if (_enemyTarget.IsValid())
	{

		if (_enemyTarget.Get()->GetFSMState() == GameStates::DEAD)
			return;
		
	
		FDamageEvent dmgEvent;
		int attack = _statComp->GetAttack();
		int dmg = FMath::RandRange(attack, attack * 2);
		

		_enemyTarget->TakeDamage(dmg , dmgEvent, GetController(), this);

		//�÷��̾�
		auto controller = Cast<AUnitPlayerController>(GetController());

		if (controller == nullptr)
			return;

		controller->PrimaryAttack_CameraShake();
	}

}

float AUnitCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	_statComp->OnAttacked(Damage);

	FVector ownedLoc = GetActorLocation();
	FVector enemyLoc = DamageCauser->GetActorLocation();
	FRotator destRot = UKismetMathLibrary::FindLookAtRotation(ownedLoc, enemyLoc);
	SetActorRotation(destRot);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FRotator rotator;
	FVector  SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 200.f;

	auto dmgActor = Cast<ADmgTextActor>(
		GetWorld()->SpawnActor<AActor>(_dmgActor, SpawnLocation, rotator, SpawnParams));

	dmgActor->UpdateDamage(Damage);

	VisibleHpBar();

	return Damage;
}


void AUnitCharacter::VisibleHpBar()
{
	UWorld* world = GetWorld();

	world->GetTimerManager().ClearTimer(_hpBarTimerHandle);

	_hpBar->SetVisibility(true);


	world->GetTimerManager().SetTimer(_hpBarTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			_hpBar->SetVisibility(false);

		}), 1.5f, false);
}

void AUnitCharacter::ChangeMinimapColor(FLinearColor color)
{
	_minimapIcon->SetSpriteColor(color);
}

void AUnitCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted)
{
	_bAttacking = false;
	_onAttackEnded.Broadcast();
}


void AUnitCharacter::FSMUpdate()
{
	switch (_gameState)
	{
	case AUnitCharacter::IDLE:

		if (_gameEvent == GameEvents::ON_ENTER)
			IdleEnter();
		
		else if (_gameEvent == GameEvents::ON_UPDATE)
			IdleUpdate();
		
		break;

	case AUnitCharacter::MOVE:

		if (_gameEvent == GameEvents::ON_ENTER)
			MoveEnter();

		else if (_gameEvent == GameEvents::ON_UPDATE)
			MoveUpdate();

		break;

	case AUnitCharacter::ATTACK:

		if (_gameEvent == GameEvents::ON_ENTER)
			AttackEnter();
	
		else if (_gameEvent == GameEvents::ON_UPDATE)
			AttackUpdate();
	
		break;

	case AUnitCharacter::DEAD:

		if (_gameEvent == GameEvents::ON_ENTER)
			DeadEnter();

		else if (_gameEvent == GameEvents::ON_UPDATE)
			DeadUpdate();
		
		break;

	default:
		break;
	}
}

void AUnitCharacter::IdleEnter()
{
	_gameEvent = GameEvents::ON_UPDATE;

	for (auto animIns : GetUnitAnim())
	{
		animIns->StopAllMontages(.1f);
	}

}

void AUnitCharacter::IdleUpdate()
{
	//TODO

}

void AUnitCharacter::IdleExit()
{
	//TODO
}

void AUnitCharacter::MoveEnter()
{
	_gameEvent = GameEvents::ON_UPDATE;

	for (auto animIns : GetUnitAnim())
	{
		animIns->StopAllMontages(.1f);
	}

}

void AUnitCharacter::MoveUpdate()
{
	ChaseTheEnemy();
}

void AUnitCharacter::MoveExit()
{

}

void AUnitCharacter::AttackEnter()
{
	_gameEvent = GameEvents::ON_UPDATE;


}

void AUnitCharacter::AttackUpdate()
{
	AttackEnemy();
}

void AUnitCharacter::AttackExit()
{
	//TODO
}

void AUnitCharacter::DeadEnter()
{
	_gameEvent = GameEvents::ON_UPDATE;

	GetWorld()->GetTimerManager().ClearTimer(_hpBarTimerHandle);

	_hpBar->SetVisibility(false);
	_outLineMesh->SetVisibility(false);


	//AI
	auto aiController = Cast<AUnitAIController>(GetController());

	if(aiController)
		aiController->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);



	GetWorld()->GetTimerManager().SetTimer(_deadHandle, FTimerDelegate::CreateLambda([&]()
		{
			auto gmInstance = Cast<UGMInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			gmInstance->DestroyEnemy(_index);

		}),2.f , false);

	//TODO : ���� ��Ȱ or ���� ������ 

}

void AUnitCharacter::DeadUpdate()
{
	
}

void AUnitCharacter::DeadExit()
{
	//TODO
}

void AUnitCharacter::SetFSMState(GameStates newState)
{
	switch (_gameState)
	{
	case AUnitCharacter::IDLE:
		IdleExit();
		break;
	case AUnitCharacter::MOVE:
		IdleExit();
		break;
	case AUnitCharacter::ATTACK:
		AttackExit();
		break;
	case AUnitCharacter::DEAD:
		DeadExit();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("������������ FSM"), newState);
		break;
	}

	_gameState = newState;
	_gameEvent = AUnitCharacter::ON_ENTER;
}