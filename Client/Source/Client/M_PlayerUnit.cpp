// Fill out your copyright notice in the Description page of Project Settings.


#include "M_PlayerUnit.h"
#include "M_Anim.h"
#include "M_UnitStat.h"
#include "M_UnitWidget.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/WidgetComponent.h>
#include <DrawDebugHelpers.h>



// Sets default values
AM_PlayerUnit::AM_PlayerUnit()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	_SpringArm->SetupAttachment(RootComponent);
	_SpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	_SpringArm->TargetArmLength = 700.f;
	_SpringArm->SetRelativeRotation(FRotator(-60.f, 45.f, 0.f));
	_SpringArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	_Cam->SetupAttachment(_SpringArm, USpringArmComponent::SocketName);
	_Cam->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	_Stat = CreateDefaultSubobject<UM_UnitStat>(TEXT("STAT"));

	_HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP_BAR"));
	_HpBar->SetupAttachment(GetMesh());
	_HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	_HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));

	static ConstructorHelpers::FClassFinder <UUserWidget> UW(TEXT("WidgetBlueprint'/Game/Resources/UI/WBP_HpBar.WBP_HpBar_C'"));

	if (UW.Succeeded())
	{
		_HpBar->SetWidgetClass(UW.Class);
		_HpBar->SetDrawSize(FVector2D(250.f, 50.f));
	}

}

void AM_PlayerUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_Anim = Cast<UM_Anim>(GetMesh()->GetAnimInstance());

	if (_Anim)
	{
		_Anim->OnMontageEnded.AddDynamic(this, &AM_PlayerUnit::OnAttackMontageEnded);
		_Anim->OnAttackHit.AddUObject(this, &AM_PlayerUnit::AttackCheck);

	}

	if (_Stat)
		_Stat->OnDead.AddUObject(this, &AM_PlayerUnit::Dead);
	
	_HpBar->InitWidget();

	auto HpWidget = Cast<UM_UnitWidget>(_HpBar->GetUserWidgetObject());

	if (HpWidget)
		HpWidget->BindHp(_Stat);

}

// Called when the game starts or when spawned
void AM_PlayerUnit::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AM_PlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AM_PlayerUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack" ,IE_Pressed, this,  &AM_PlayerUnit::Attack);

}

void AM_PlayerUnit::Attack()
{
	if (_bAttacking || _bDead)
		return;

	_Anim->PlayAttackMontage();

	_bAttacking = true;

}

void AM_PlayerUnit::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	//타격 판정
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;

	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		Rotation, DrawColor, false, 2.f);


	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;

		HitResult.Actor->TakeDamage(_Stat->GetAttack(), DamageEvent, GetController(), this);
	}
}

void AM_PlayerUnit::Dead()
{
	_bDead = true;
	_Anim->PlayDeadAnim();
}

void AM_PlayerUnit::OnAttackMontageEnded(UAnimMontage* Montage, bool bInteruppted)
{
	_bAttacking = false;
}

float AM_PlayerUnit::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	_Stat->OnAttacked(Damage);

	return Damage;
}



