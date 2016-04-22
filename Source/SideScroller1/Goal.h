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

UPROPERTY(EditAnywhere, Category = "Level")
bool bIsActive;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxScore;
	// Sets default values for this actor's properties
	AGoal();
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
	void LoadNextLevel();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure, Category = "Level")
	bool GetIsActive();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void SetIsActive(bool bSetActive);
};
