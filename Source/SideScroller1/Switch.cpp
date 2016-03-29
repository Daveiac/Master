// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSpriteComponent.h"
#include "Switch.h"

// Sets default values
ASwitch::ASwitch()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> OffSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> OnSprite;
		FConstructorStatics()
			: OffSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/switchMid_Sprite.switchMid_Sprite'"))
			, OnSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/switchLeft_Sprite.switchLeft_Sprite'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	OffSprite = ConstructorStatics.OffSprite.Get();
	OnSprite = ConstructorStatics.OnSprite.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(OffSprite);
	SpriteComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SpriteComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = SpriteComponent;

	bIsActivated = false;
	bIsImpulseSwitch = false;
	ImpulseTimer = 0.0f;
}

void ASwitch::Activate()
{
	bIsActivated = true;
	SpriteComponent->SetSprite(OnSprite);
	if (bIsImpulseSwitch)
	{
		FTimerHandle Impulse;
		GetWorldTimerManager().SetTimer(Impulse, this, &ASwitch::Deactivate, ImpulseTimer);
	}
}

void ASwitch::Deactivate()
{
	bIsActivated = false;
	SpriteComponent->SetSprite(OffSprite);
}

void ASwitch::NotifyActorOnClicked()
{
	(bIsActivated) ? Deactivate() : Activate();
}
