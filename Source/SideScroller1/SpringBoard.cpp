// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSpriteComponent.h"
#include "SideScroller1Character.h"
#include "Kismet/GameplayStatics.h"
#include "SpringBoard.h"


// Sets default values
ASpringBoard::ASpringBoard()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> UpSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> DownSprite;
		FConstructorStatics()
			: UpSprite(TEXT("/Game/2DSideScroller/Sprites/Items/springboardUp_Sprite.springboardUp_Sprite"))
			, DownSprite(TEXT("/Game/2DSideScroller/Sprites/Items/springboardDown_Sprite.springboardDown_Sprite"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	UpSprite = ConstructorStatics.UpSprite.Get();
	DownSprite = ConstructorStatics.DownSprite.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(UpSprite);
	SpriteComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = SpriteComponent;
	bIsReady = true;

	SpringDelay = 0.2f;
	SpringFactor = 1.5f;
}

void ASpringBoard::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other != nullptr && Other != this)
	{
		// Check if the OtherActor is a Character
		if (ASideScroller1Character *Character = Cast<ASideScroller1Character>(Other))
		{
			if (HitNormal.Z < -0.1 && bIsReady)
			{
				Prime();
			}
		}
	}
}

void ASpringBoard::Prime()
{
	bIsReady = false;
	SpriteComponent->SetSprite(DownSprite);
	FTimerHandle SpringTimer;
	GetWorldTimerManager().SetTimer(SpringTimer, this, &ASpringBoard::Spring, SpringDelay);
}

void ASpringBoard::Spring()
{
	SpriteComponent->SetSprite(UpSprite);
	bIsReady = true;
	ACharacter *Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Character->LaunchCharacter(FVector(0, 0, Character->GetCharacterMovement()->JumpZVelocity * SpringFactor), false, false);
}
