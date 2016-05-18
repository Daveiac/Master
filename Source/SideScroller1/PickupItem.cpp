// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller1GameMode.h"
#include "SideScroller1Character.h"
#include "PickupItem.h"


// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> Sprite;
		ConstructorHelpers::FObjectFinderOptional<USoundWave> PickupSound;
		FConstructorStatics()
			: Sprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/coinGold_Sprite.coinGold_Sprite'"))
			, PickupSound(TEXT("SoundWave'/Game/2DSideScroller/Sound/SFX/coin.coin'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	Sprite = ConstructorStatics.Sprite.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(Sprite);
	SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = SpriteComponent;

	PickupSound = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("PickupSound"));
	PickupSound->SetSound(ConstructorStatics.PickupSound.Get());
	PickupSound->bAutoActivate = false;
	PickupSound->bStopWhenOwnerDestroyed = false;

	ScoreValue = 1;
}

// Called every frame
void APickupItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupItem::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (ASideScroller1GameMode *GameMode = Cast<ASideScroller1GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ASideScroller1Character *Character = Cast<ASideScroller1Character>(OtherActor))
		{
			GameMode->AddToScore(ScoreValue);
			PickupSound->Play();
			Destroy();
		}
	}
}

