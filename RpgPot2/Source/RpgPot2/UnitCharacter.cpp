// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/WidgetComponent.h>
#include "UnitPlayerController.h"
#include "UnitAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "DmgTextActor.h"
#include "HpBarWidget.h"
#include "StatDataComponent.h"


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
	_outLineMesh->SetupAttachment(RootComponent);
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

	//_minimapspring = createdefaultsubobject<uspringarmcomponent>(text("minimap_spring_arm"));
	//_minimapspring->setupattachment(getcapsulecomponent());
	//_minimapspring->setusingabsoluterotation(true);
	//_minimapspring->targetarmlength = 300.f;
	//_minimapspring->setrelativerotation(frotator(0.f, -90.f, 0.f));
	//_minimapspring->bdocollisiontest = false;
}

// Called when the game starts or when spawned
void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_animIns = Cast<UUnitAnim>(GetMesh()->GetAnimInstance());

	if (_animIns)
	{
		_animIns->OnMontageEnded.AddDynamic(this, &AUnitCharacter::OnAttackMontageEnded);
		_animIns->GetOnAttackHit().AddUObject(this, &AUnitCharacter::AttackCheck);
	}

	_hpBar->InitWidget();
	_hpBar->SetVisibility(false);

	auto hpBarWidget = Cast<UHpBarWidget>(_hpBar->GetUserWidgetObject());
	if (hpBarWidget)
		hpBarWidget->BindHp(_statComp);

}

// Called every frame
void AUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PC = Cast<AUnitPlayerController>(GetController());

	if (PC)
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Pawn, true, HitResult);

		
		if (HitResult.bBlockingHit)
		{
			auto Obj = Cast<AUnitCharacter>(HitResult.Actor);
			PC->CheckEnemy(Obj, this);
		}
	}

}

// Called to bind functionality to input
void AUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnitCharacter::Attack()
{
	if (_bAttacking)
		return;

	_animIns->PlayAttackMontage();

	_bAttacking = true;
	
}

void AUnitCharacter::AttackCheck()
{

	auto controller = Cast<AUnitPlayerController>(GetController());

	if (controller == nullptr)
		return;

	auto target = controller->GetTarget();

	if (target.IsValid())
	{
		FDamageEvent dmgEvent;
		int attack = _statComp->GetAttack();
		int dmg = FMath::RandRange(attack, attack * 2);
		
		target->TakeDamage(dmg , dmgEvent, GetController(), this);
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

	world->GetTimerManager().ClearTimer(timerHandle);

	_hpBar->SetVisibility(true);


	world->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			_hpBar->SetVisibility(false);

		}), 1.5f, false);
}

void AUnitCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted)
{
	_bAttacking = false;
	_onAttackEnded.Broadcast();
}

