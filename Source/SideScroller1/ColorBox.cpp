// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "ColorBox.h"


// Sets default values
AColorBox::AColorBox()
{
	PrimaryActorTick.bCanEverTick = true;
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> BoxOn;
		FConstructorStatics()
			: BoxOn(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/weight_Sprite.weight_Sprite'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(ConstructorStatics.BoxOn.Get());
	RootComponent = SpriteComponent;
	SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Channel = 0;
	FadeState = Normal;
	bIsActive = true;
}

void AColorBox::Tick(float DeltaSeconds)
{
	if (FadeState != Normal)
	{
		float FadeRate = (FadeState == FadeIn)? 1.f * DeltaSeconds : -1.f * DeltaSeconds;
		FLinearColor SpriteColor = SpriteComponent->GetSpriteColor();
		SpriteColor.A += FadeRate;
		if (SpriteColor.A > 1.0f)
		{
			SpriteColor.A = 1.0f;
			FadeState = Normal;
			SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		}
		else if (SpriteColor.A < 0)
		{
			SpriteColor.A = 0.0f;
			FadeState = Normal;
			SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		}
		SpriteComponent->SetSpriteColor(SpriteColor);
	}
}

void AColorBox::Activate()
{
	FadeState = FadeIn;
	bIsActive = true;
}

void AColorBox::Deactivate()
{
	FadeState = FadeOut;
	bIsActive = false;
}
