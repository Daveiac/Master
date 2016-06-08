// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "MainCharacter.h"
#include "KillVolume.h"

void AKillVolume::NotifyActorBeginOverlap(AActor *OtherActor)
{
	if (AMainCharacter *Character = Cast<AMainCharacter>(OtherActor))
	{
		Character->Kill();
	}
}




