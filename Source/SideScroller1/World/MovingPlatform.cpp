// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperSpriteComponent.h"
#include "MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> Sprite;
		FConstructorStatics()
			: Sprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/MovingPlatform_Sprite.MovingPlatform_Sprite'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	Sprite = ConstructorStatics.Sprite.Get();
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetSprite(Sprite);
	RootComponent = SpriteComponent;

	PlatformSpeed = 200.0f;
}

void AMovingPlatform::BeginPlay()
{
	CurrentTarget = Target1;
}

// Called every frame
void AMovingPlatform::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bIsActive)
	{
		if (GetActorLocation().Equals(Target1, 2.0f)) CurrentTarget = Target2;
		else if (GetActorLocation().Equals(Target2, 2.0f)) CurrentTarget = Target1;
		FVector Direction = CurrentTarget - GetActorLocation();
		Direction.Normalize();
		Direction *= PlatformSpeed * DeltaTime;
		AddActorWorldTransform(FTransform(Direction));
	}
}

