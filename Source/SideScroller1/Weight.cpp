// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSpriteComponent.h"
#include "Weight.h"


// Sets default values
AWeight::AWeight()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> WeigthFalling;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> WeightHanging;
		FConstructorStatics()
			: WeigthFalling(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/weight_Sprite.weight_Sprite'"))
			, WeightHanging(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/weightChained_Sprite.weightChained_Sprite'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	FallingSprite = ConstructorStatics.WeigthFalling.Get();
	HangingSprite = ConstructorStatics.WeightHanging.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(HangingSprite);
}