// Fill out your copyright notice in the Description page of Project Settings.


#include "M_PlayerUnit.h"
#include "M_Anim.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/WidgetComponent.h>


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
	_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	_SpringArm->SetupAttachment(RootComponent);
	_SpringArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	_SpringArm->TargetArmLength = 700.f;
	_SpringArm->SetRelativeRotation(FRotator(-60.f, 45.f, 0.f));
	_SpringArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	_Cam->SetupAttachment(_SpringArm, USpringArmComponent::SocketName);
	_Cam->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	_HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP_BAR"));
	_HpBar->SetupAttachment(GetMesh());
	_HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	_HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));

	static ConstructorHelpers::FClassFinder <UUserWidget> UW(TEXT("WidgetBlueprint'/Game/Resources/UI/WBP_HpBar.WBP_HpBar_C'"));

	if (UW.Succeeded())
	{
		_HpBar->SetWidgetClass(UW.Class);
		_HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}
	
}

void AM_PlayerUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_Anim = Cast<UM_Anim>(GetMesh()->GetAnimInstance());

	if (_Anim)
	{
		_Anim->OnMontageEnded.AddDynamic(this, &AM_PlayerUnit::OnAttackMontageEnded);
		_Anim->GetOnAttackHit().AddUObject(this, &AM_PlayerUnit::AttackCheck);

	}
	
	_HpBar->InitWidget();
}

// Called when the game starts or when spawned
void AM_PlayerUnit::BeginPlay()
{
	Super::BeginPlay();


	auto MyController = Cast<AController>(GetController());
	
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
	if (_bAttacking)
		return;

	_Anim->PlayAttackMontage();

	_bAttacking = true;

}

void AM_PlayerUnit::AttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Check"));
}

void AM_PlayerUnit::OnAttackMontageEnded(UAnimMontage* Montage, bool bInteruppted)
{
	_bAttacking = false;
}

