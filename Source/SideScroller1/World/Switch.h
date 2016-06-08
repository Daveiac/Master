// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Switch.generated.h"

UCLASS()
class SIDESCROLLER1_API ASwitch : public AActor
{
GENERATED_BODY()

UPROPERTY(EditAnywhere, Category = "Switch")
bool bIsImpulseSwitch;

UPROPERTY(EditAnywhere, Category = "Switch")
float ImpulseTimer;

UPROPERTY(EditAnywhere, Category = "Switch")
class UPaperSpriteComponent *SpriteComponent;

UPROPERTY(EditAnywhere, Category = "Switch")
class UPaperSprite *OffSprite;

UPROPERTY(EditAnywhere, Category = "Switch")
class UPaperSprite *OnSprite;

UPROPERTY(EditAnywhere, Category = "Switch")
TArray<class AMovingPlatform*> Platforms;

virtual void NotifyActorOnClicked() override;

void UpdatePlatforms();
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	bool bIsActivated;
	
	// Sets default values for this actor's properties
	ASwitch();
	UFUNCTION(BlueprintCallable, Category = "Switch")
	virtual void Activate();
	UFUNCTION(BlueprintCallable, Category = "Switch")
	virtual void Deactivate();

	

protected:
	static const FString OffSpriteAssetName;
	static const FString OnSpriteAssetName;
};
