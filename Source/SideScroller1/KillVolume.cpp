// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "SideScroller1Character.h"
#include "KillVolume.h"

void AKillVolume::NotifyActorBeginOverlap(AActor *OtherActor)
{
	if (ASideScroller1Character *Character = Cast<ASideScroller1Character>(OtherActor))
	{
		Character->Kill();
	}
}




