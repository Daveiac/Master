// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "ColorBox.h"
#include "ColorSwitch.h"

void AColorSwitch::Activate()
{
	Super::Activate();
	TTransArray<AActor*> Actors = GetLevel()->Actors;
	for (int i = 0; i < Actors.Num(); i++)
	{
		if (AColorBox *ColorBox = Cast<AColorBox>(Actors[i]))
		{
			if (ColorBox->Channel == Channel)
				ColorBox->Deactivate();
		}
	}
}

void AColorSwitch::Deactivate()
{
	Super::Deactivate();
	TTransArray<AActor*> Actors = GetLevel()->Actors;
	for (int i = 0; i < Actors.Num(); i++)
	{
		if (AColorBox *ColorBox = Cast<AColorBox>(Actors[i]))
		{
			if (ColorBox->Channel == Channel)
				ColorBox->Activate();
		}
	}
}
