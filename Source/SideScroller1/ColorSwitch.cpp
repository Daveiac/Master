// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "ColorBox.h"
#include "ColorSwitch.h"

void AColorSwitch::Activate()
{
	Toggle();
}

void AColorSwitch::Deactivate()
{
	Toggle();
}

void AColorSwitch::Toggle()
{
	(bIsActivated)? Super::Deactivate() : Super::Activate();
	TTransArray<AActor*> Actors = GetLevel()->Actors;
	for (int i = 0; i < Actors.Num(); i++)
	{
		if (AColorBox *ColorBox = Cast<AColorBox>(Actors[i]))
		{
			if (ColorBox->Channel == Channel)
			{

				void(AColorBox::*FunctionColorBox)(void) = (ColorBox->bIsActive) ? &AColorBox::Deactivate : &AColorBox::Activate;
				(ColorBox->*FunctionColorBox)();
			}
		}
	}
}
