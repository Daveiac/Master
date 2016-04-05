// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSpriteComponent.h"
#include "MovingPlatform.h"
#include "Switch.h"

const FString ASwitch::OffSpriteAssetName = "PaperSprite'/Game/2DSideScroller/Sprites/Items/switchMid_Sprite.switchMid_Sprite'";
const FString ASwitch::OnSpriteAssetName = "PaperSprite'/Game/2DSideScroller/Sprites/Items/switchLeft_Sprite.switchLeft_Sprite'";

ASwitch::ASwitch()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> OffSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> OnSprite;
		FConstructorStatics()
			: OffSprite(*OffSpriteAssetName)
			, OnSprite(*OnSpriteAssetName)
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
	Platforms = TArray<AMovingPlatform*>();
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
	UpdatePlatforms();
}

void ASwitch::Deactivate()
{
	bIsActivated = false;
	SpriteComponent->SetSprite(OffSprite);
	UpdatePlatforms();
}

void ASwitch::NotifyActorOnClicked()
{
	(bIsActivated) ? Deactivate() : Activate();
	
}

void ASwitch::UpdatePlatforms()
{
	if (Platforms.Num())
	{
		for (int i = 0; i < Platforms.Num(); i++)
		{
			AMovingPlatform *Platform = Platforms[i];
			Platform->bIsActive = bIsActivated;
		}
	}
}
