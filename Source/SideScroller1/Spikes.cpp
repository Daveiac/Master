// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "MainCharacter.h"
#include "PaperSpriteComponent.h"
#include "Spikes.h"



// Sets default values
ASpikes::ASpikes()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> Sprite;
		ConstructorHelpers::FObjectFinderOptional<USoundWave> SpikeHitSound;
		FConstructorStatics()
			: Sprite(TEXT("/Game/2DSideScroller/Sprites/Items/spikes_Sprite.spikes_Sprite"))
			, SpikeHitSound(TEXT("SoundWave'/Game/2DSideScroller/Sound/SFX/hit_spike.hit_spike'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	Sprite = ConstructorStatics.Sprite.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(Sprite);
	SpriteComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = SpriteComponent;

	SpikeHitSound = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("SpikeHitSound"));
	SpikeHitSound->SetSound(ConstructorStatics.SpikeHitSound.Get());
	SpikeHitSound->bAutoActivate = false;
}

void ASpikes::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other != nullptr && Other != this)
	{
		// Check if the OtherActor is a Character
		if (AMainCharacter *Character = Cast<AMainCharacter>(Other))
		{
			if (HitNormal.Z < -0.1)
			{
				SpikeHitSound->Play();
				Character->DamageCharacter(2);
				Character->LaunchCharacter(FVector(0, 0, Character->GetCharacterMovement()->JumpZVelocity * 0.7f), false, false);
			}
		}
	}
}