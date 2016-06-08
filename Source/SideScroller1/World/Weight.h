// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weight.generated.h"

UCLASS()
class SIDESCROLLER1_API AWeight : public AActor
{
	GENERATED_BODY()

class UPaperSpriteComponent *SpriteComponent;
class UPaperSprite *HangingSprite;
class UPaperSprite *FallingSprite;

public:	
	// Sets default values for this actor's properties
	AWeight();
};
