// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class SIDESCROLLER1_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, Category = "Sprite")
class UPaperSpriteComponent *SpriteComponent;
class UPaperSprite *Sprite;
UPROPERTY(VisibleAnywhere, Category = "Moving Platform")
FVector CurrentTarget;
UPROPERTY(EditAnywhere, Category = "Moving Platform")
float PlatformSpeed;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Platform")
	bool bIsActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Platform")
	FVector Target1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving Platform")
	FVector Target2;


	// Sets default values for this actor's properties
	AMovingPlatform();
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void BeginPlay() override;
};
