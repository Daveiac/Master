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

enum FadingState { Normal, FadeIn, FadeOut };
FadingState FadeState;

public:
	UPROPERTY(EditAnywhere, Category = "ColorBox")
	int32 Channel;
	bool bIsActive;
	AColorBox();
	virtual void Tick(float DeltaSeconds) override;
	void Activate();
	void Deactivate();
	
};
