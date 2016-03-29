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

virtual void NotifyActorOnClicked() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	bool bIsActivated;
	
	// Sets default values for this actor's properties
	ASwitch();
	UFUNCTION(BlueprintCallable, Category = "Switch")
	void Activate();
	UFUNCTION(BlueprintCallable, Category = "Switch")
	void Deactivate();
};
