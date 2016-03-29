// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "SideScroller1Character.h"
#include "PaperSpriteComponent.h"
#include "Spikes.h"



// Sets default values
ASpikes::ASpikes()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> Sprite;
		FConstructorStatics()
			: Sprite(TEXT("/Game/2DSideScroller/Sprites/Items/spikes_Sprite.spikes_Sprite"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	Sprite = ConstructorStatics.Sprite.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(Sprite);
	SpriteComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = SpriteComponent;
}

void ASpikes::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other != nullptr && Other != this)
	{
		// Check if the OtherActor is a Character
		if (ASideScroller1Character *Character = Cast<ASideScroller1Character>(Other))
		{
			if (HitNormal.Z < -0.1)
			{
				Character->TakeDamage(3);
				Character->LaunchCharacter(FVector(0, 0, Character->GetCharacterMovement()->JumpZVelocity / 2.0f), false, false);
			}
		}
	}
}