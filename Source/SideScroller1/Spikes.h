// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spikes.generated.h"

UCLASS()
class SIDESCROLLER1_API ASpikes : public AActor
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, Category = "Sprite")
class UPaperSpriteComponent *SpriteComponent;

UPROPERTY(EditAnywhere, Category = "Sprite")
class UPaperSprite *Sprite;


public:	
	// Sets default values for this actor's properties
	ASpikes();

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
};
