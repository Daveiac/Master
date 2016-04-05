// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ColorBox.generated.h"

UCLASS()
class SIDESCROLLER1_API AColorBox : public AActor
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, Category = "Sprite")
class UPaperSpriteComponent *SpriteComponent;

public:
	UPROPERTY(EditAnywhere, Category = "ColorBox")
	int32 Channel;
	AColorBox();
	void Activate();
	void Deactivate();
	
};
