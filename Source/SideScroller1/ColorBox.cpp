// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "ColorBox.h"


// Sets default values
AColorBox::AColorBox()
{
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
}

void AColorBox::Activate()
{
	SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SpriteComponent->SetVisibility(true);
}

void AColorBox::Deactivate()
{
	SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SpriteComponent->SetVisibility(false);
}
