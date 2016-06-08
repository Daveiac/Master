// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "KillVolume.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER1_API AKillVolume : public ATriggerBox
{
	GENERATED_BODY()

public:
	void NotifyActorBeginOverlap(AActor *OtherActor) override;

};
