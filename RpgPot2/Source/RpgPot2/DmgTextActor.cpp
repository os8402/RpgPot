// Fill out your copyright notice in the Description page of Project Settings.


#include "DmgTextActor.h"
#include <Components/WidgetComponent.h>
#include "DmgTextWidget.h"


// Sets default values
ADmgTextActor::ADmgTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_dmgText = CreateDefaultSubobject<UWidgetComponent>(TEXT("DMG_TEXT"));
	_dmgText->SetupAttachment(RootComponent);
	_dmgText->SetWidgetSpace(EWidgetSpace::Screen);

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

void ADmgTextActor::UpdateDamage(int32 dmg)
{
	auto dmgWidget = Cast<UDmgTextWidget>(_dmgText->GetUserWidgetObject());
	dmgWidget->UpdateText(dmg);
}

