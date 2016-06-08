// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class SIDESCROLLER1_API ACoin : public AActor
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, Category = "Score")
int32 ScoreValue;

UPROPERTY(EditAnywhere, Category = "Sprite")
class UPaperSpriteComponent *SpriteComponent;
class UPaperSprite *Sprite;

UPROPERTY(EditAnywhere, Category = "Sound")
UAudioComponent *PickupSound;
public:	
	// Sets default values for this actor's properties
	ACoin();
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
};
