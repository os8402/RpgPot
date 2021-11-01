// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPlayer.h"
#include <Components/CapsuleComponent.h>


// Sets default values
AUnitPlayer::AUnitPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//_springArm = CreateDefaultSubobject<USpringArmCompoenent>(TEXT("SPRING_ARM"));

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