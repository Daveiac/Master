// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UCLASS()
class SIDESCROLLER1_API AGoal : public AActor
{
	GENERATED_BODY()

class UPaperFlipbookComponent *FlipbookComponent;
class UPaperFlipbook *ActiveFlipbook;
class UPaperFlipbook *InactiveFlipbook;
UAudioComponent *LevelCompletedSound;

UPROPERTY(EditAnywhere, Category = "Level")
TAssetPtr<UWorld> NextLevel;

public:
	bool bIsActive;
	// Sets default values for this actor's properties
	AGoal();
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
	void LoadNextLevel();
};
