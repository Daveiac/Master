// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "Bait.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"


// Sets default values
ABait::ABait()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BaitFB;
		FConstructorStatics()
			: BaitFB(TEXT("PaperFlipbook'/Game/2DSideScroller/Sprites/Items/cherry_Flipbook.cherry_Flipbook'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	BaitFB = ConstructorStatics.BaitFB.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(BaitFB->GetSpriteAtFrame(0));
	SpriteComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SpriteComponent;
	CurrentFrame = 0;
	bIsEdible = true;
}

void ABait::BeEaten(int32 FrameSkip = 1)
{
	if (bIsEdible && FrameSkip > 0)
	{
		int32 LastFrame = BaitFB->GetNumFrames() - 1;
		int32 NextFrame = FMath::Min(CurrentFrame + FrameSkip, LastFrame);
		CurrentFrame = NextFrame;
		if (NextFrame == LastFrame)
			bIsEdible = false;
		SpriteComponent->SetSprite(BaitFB->GetSpriteAtFrame(CurrentFrame));
	}
}

