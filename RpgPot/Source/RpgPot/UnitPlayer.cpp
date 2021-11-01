// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayer.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera//CameraComponent.h>



// Sets default values
AUnitPlayer::AUnitPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//√ ±‚»≠
	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING_ARM"));
	_springArm->SetupAttachment(GetCapsuleComponent());
	_springArm->TargetArmLength = _targetArmLength;
	_springArm->SetRelativeRotation(FRotator(0.f , -35.f , 0.f));
	
	_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAM"));
	_cam->SetupAttachment(_springArm);
	_cam->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));



}

// Called when the game starts or when spawned
void AUnitPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnitPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AUnitPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnitPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AUnitPlayer::Attack()
{


}


void AUnitPlayer::AttackCheck()
{


}