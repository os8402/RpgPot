// Fill out your copyright notice in the Description page of Project Settings.


#include "M_PlayerUnit.h"
#include "M_Anim.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>


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

