// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Item.h"
#include "Bait.generated.h"

UCLASS()
class SIDESCROLLER1_API ABait : public AItem
{
	GENERATED_BODY()

class UPaperFlipbook *BaitFB;
int32 CurrentFrame;

protected:

	virtual void LoadAsstets();

public:	
	bool bIsEdible;
	// Sets default values for this actor's properties
	ABait();
	void BeEaten(int32 frameskip);
	virtual void Grab();
	virtual bool Release();
};
