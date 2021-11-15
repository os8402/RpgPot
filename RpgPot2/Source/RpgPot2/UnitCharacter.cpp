// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
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
			
			//아웃라인 메시는 공격판정 해제
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
	{
		FString chrName = TEXT("악의 무리");
		
		FString barInfoStr = FString::Printf(TEXT("%s LV %d"), *chrName, 1);

		hpBarWidget->BindHp(FText::FromString(barInfoStr) , _statComp);
	}
	

	//처음엔 빨강으로 고정
	ChangeMinimapColor(FLinearColor(1.f, 0.f, 0.f, 1.f));

}

// Called every frame
void AUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FSMUpdate();

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

		if (this->GetDistanceTo(_enemyTarget.Get()) > 300.f)
			return;
		
	
		FDamageEvent dmgEvent;
		int attack = _statComp->GetAttack();
		int dmg = FMath::RandRange(attack, attack * 2);
		
		_enemyTarget->TakeDamage(dmg , dmgEvent, GetController(), this);

	}

}

float AUnitCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	_statComp->OnAttacked(Damage);

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



void AUnitCharacter::DeadCharacter()
{
	_hpBar->SetVisibility(false);
	_outLineMesh->SetVisibility(false);

	GetWorld()->GetTimerManager().ClearTimer(_hpBarTimerHandle);


	//TODO : 마을 부활 or 게임 끝내기 
}

void AUnitCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted)
{
	_bAttacking = false;
	_onAttackEnded.Broadcast();
}


//


void AUnitCharacter::SetFSMState(GameStates newState)
{

	_gameState = newState;

	switch (_gameState)
	{
	case AUnitCharacter::IDLE:

		break;

	case AUnitCharacter::ATTACK:
	
		break;
	case AUnitCharacter::DEAD:

	
		DeadCharacter();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("존재하지않은 FSM"), newState);
		break;
	}

	
}

//void AUnitCharacter::FSMUpdate()
////{
////	switch (_gameState)
////	{
////	case AUnitCharacter::IDLE:
////
////			IdleUpdate();
////		
////		break;
////
////	case AUnitCharacter::ATTACK:
////
////			AttackUpdate();
////	
////		break;
////
////	case AUnitCharacter::DEAD:
////			DeadUpdate();
////		
////		break;
////
////	default:
////		break;
////	}
////}
//
//
//void AUnitCharacter::IdleUpdate()
//{
//
//
//}
//
//void AUnitCharacter::AttackUpdate()
//{
//
//}
//void AUnitCharacter::DeadUpdate()
//{
//	
//}