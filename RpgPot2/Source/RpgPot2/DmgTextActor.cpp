// Fill out your copyright notice in the Description page of Project Settings.


#include "DmgTextActor.h"
#include <Components/WidgetComponent.h>
#include "DmgTextWidget.h"
#include <Components/CapsuleComponent.h>


// Sets default values
ADmgTextActor::ADmgTextActor()
{
 
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* defalutRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMP"));

	RootComponent = defalutRoot;

	_meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	_meshComp->SetupAttachment(defalutRoot);
	_meshComp->SetRelativeLocation(FVector::ZeroVector);

	_dmgText = CreateDefaultSubobject<UWidgetComponent>(TEXT("DMG_TEXT"));
	_dmgText->SetupAttachment(_meshComp);
	_dmgText->SetWidgetSpace(EWidgetSpace::Screen);
	_dmgText->SetRelativeLocation(FVector(200.f, 0.f , 0.f));


	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WBP_DmgText.WBP_DmgText_C'"));

	if (UW.Succeeded())
	{
		_dmgText->SetWidgetClass(UW.Class);
		_dmgText->SetDrawSize(FVector2D(200.f, 50.f));
	}
}


void ADmgTextActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_dmgText->InitWidget();

}

// Called every frame
void ADmgTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_curTime += DeltaTime;

	if (_curTime >= _destroyTime)
	{
		UWorld* world = GetWorld();
		if (world)
			world->DestroyActor(this);
	}

}

void ADmgTextActor::UpdateDamage(int32 dmg, FLinearColor color)
{
	auto dmgWidget = Cast<UDmgTextWidget>(_dmgText->GetUserWidgetObject());
	dmgWidget->UpdateText(dmg, color);
}

