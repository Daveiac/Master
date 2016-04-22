// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "Bait.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"


void ABait::LoadAsstets()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BaitFB;
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> InventoryIcon;
		FConstructorStatics()
			: BaitFB(TEXT("PaperFlipbook'/Game/2DSideScroller/Sprites/Items/cherry_Flipbook.cherry_Flipbook'"))
			, InventoryIcon(TEXT("Texture2D'/Game/2DSideScroller/Sprites/Items/cherry.cherry'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	BaitFB = ConstructorStatics.BaitFB.Get();
	InventoryIcon = ConstructorStatics.InventoryIcon.Get();
	PickupSprite = BaitFB->GetSpriteAtFrame(0);
	PlacementSprite = BaitFB->GetSpriteAtFrame(0);
	PaperSpriteComponent->SetSprite(PickupSprite);
}

// Sets default values
ABait::ABait()
	: AItem()
{
	LoadAsstets();
	CurrentFrame = 0;
	bIsEdible = false;
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
		PaperSpriteComponent->SetSprite(BaitFB->GetSpriteAtFrame(CurrentFrame));
	}
}

bool ABait::Release()
{
	bool RetVal = AItem::Release();
	if (RetVal)
	{
		PaperSpriteComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		bIsEdible = true;
	}
	return RetVal;
}

