// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperFlipbookComponent.h"
#include "MainCharacter.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Goal.h"


// Sets default values
AGoal::AGoal()
{
	PrimaryActorTick.bCanEverTick = true;
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> Active;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> Inactive;
		ConstructorHelpers::FObjectFinderOptional<USoundWave> LevelCompleted;
		FConstructorStatics()
			: Active(TEXT("PaperFlipbook'/Game/2DSideScroller/Sprites/WorldTiles/Flag.Flag'"))
			, Inactive(TEXT("PaperFlipbook'/Game/2DSideScroller/Sprites/WorldTiles/FlagHanging.FlagHanging'"))
			, LevelCompleted(TEXT("SoundWave'/Game/2DSideScroller/Sound/SFX/win.win'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	ActiveFlipbook = ConstructorStatics.Active.Get();
	InactiveFlipbook = ConstructorStatics.Inactive.Get();
	FlipbookComponent = CreateAbstractDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetFlipbook(ActiveFlipbook);
	FlipbookComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = FlipbookComponent;

	LevelCompletedSound = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("LevelCompletedSound"));
	LevelCompletedSound->SetSound(ConstructorStatics.LevelCompleted.Get());
	LevelCompletedSound->bAutoActivate = false;

	bIsActive = true;
	MaxScore = 0;
}

void AGoal::Tick(float DeltaSeconds)
{
	if (!bIsActive)
	{
		if (AMainGameMode *GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			if (GameMode->Score >= MaxScore)
				SetIsActive(true);
		}
	}
}

void AGoal::NotifyActorBeginOverlap(AActor *OtherActor)
{
	if (bIsActive)
	{
		if (AMainCharacter *Character = Cast<AMainCharacter>(OtherActor))
		{
			LevelCompletedSound->Play();
			Character->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			FTimerHandle LoadNextLevelTimer;
			GetWorldTimerManager().SetTimer(LoadNextLevelTimer, this, &AGoal::LoadNextLevel, 2.0f);
		}
	}
}

void AGoal::LoadNextLevel()
{
	FString PathName = NextLevel.ToString();
	if (GetWorld()->IsPlayInEditor())
	{
		int32 index;
		PathName.FindLastChar('.', index);
		FString LevelName = *PathName.RightChop(index + 1);
		UGameplayStatics::OpenLevel(this, *LevelName);
	}
	else
	{
		//UWorld *World = NextLevel.LoadSynchronous();
		UGameplayStatics::OpenLevel(this, *PathName);
	}
}

void AGoal::BeginPlay()
{
	if (!bIsActive)
	{
		FlipbookComponent->SetFlipbook(InactiveFlipbook);
	}
}

bool AGoal::GetIsActive()
{
	return bIsActive;
}

void AGoal::SetIsActive(bool bSetActive)
{
	UPaperFlipbook *FlipBook = (bSetActive) ? ActiveFlipbook : InactiveFlipbook;
	FlipbookComponent->SetFlipbook(FlipBook);
	bIsActive = bSetActive;
}
