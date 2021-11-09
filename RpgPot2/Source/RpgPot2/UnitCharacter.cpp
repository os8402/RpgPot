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
			//UE_LOG(LogTemp, Log, TEXT("Hit Actor %s"), *HitResult.Actor->GetName());

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
		target->Damage(this);	
	}

	

}

void AUnitCharacter::Damage(AUnitCharacter* Attacker)
{

	FVector ownedLoc = GetActorLocation();
	FVector enemyLoc = Attacker->GetActorLocation();
	FRotator destRot = UKismetMathLibrary::FindLookAtRotation(ownedLoc, enemyLoc);
	SetActorRotation(destRot);

	int32 dmg = FMath::RandRange(10, 100);

	_hp = FMath::Max(0, _hp - dmg);

	UE_LOG(LogTemp, Log, TEXT("Hp %d") , _hp);

	UWorld* world = GetWorld();

	if (world == nullptr)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FRotator rotator;
	FVector  SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 50.f;

	auto dmgActor = Cast<ADmgTextActor>(
		world->SpawnActor<AActor>(_dmgActor, SpawnLocation, rotator, SpawnParams));

	dmgActor->UpdateDamage(dmg);

}

void AUnitCharacter::OnAttackMontageEnded(UAnimMontage* montage, bool bInteruppted)
{
	_bAttacking = false;
	_onAttackEnded.Broadcast();
}

