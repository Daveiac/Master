// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bait.generated.h"

UCLASS()
class SIDESCROLLER1_API ABait : public AActor
{
	GENERATED_BODY()

class UPaperSpriteComponent *SpriteComponent;
class UPaperFlipbook *BaitFB;
int32 CurrentFrame;

public:	
	bool bIsEdible;
	// Sets default values for this actor's properties
	ABait();
	void BeEaten(int32 frameskip);
};
