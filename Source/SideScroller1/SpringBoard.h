// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpringBoard.generated.h"

UCLASS()
class SIDESCROLLER1_API ASpringBoard : public AActor
{
	GENERATED_BODY()
UPROPERTY(EditAnywhere, Category = "Sprite")
class UPaperSpriteComponent *SpriteComponent;
class UPaperSprite *UpSprite;
class UPaperSprite *DownSprite;
bool bIsReady;

UPROPERTY(EditAnywhere, Category = "SpringBoard")
float SpringDelay;
UPROPERTY(EditAnywhere, Category = "SpringBoard")
float SpringFactor;


public:	
	// Sets default values for this actor's properties
	ASpringBoard();

	virtual void NotifyHit
	(
		class UPrimitiveComponent * MyComp,
		class AActor * Other,
		class UPrimitiveComponent * OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult & Hit
	) override;

	// Primes the springboard in the down position
	void Prime();

	// Activates the springboard and launches whatever is on
	void Spring();
};
