// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideScroller1.h"
#include "SideScroller1GameMode.h"
#include "SideScroller1Character.h"
#include "Blueprint/UserWidget.h"

ASideScroller1GameMode::ASideScroller1GameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = ASideScroller1Character::StaticClass();

	// Set Tickable
	PrimaryActorTick.bCanEverTick = true;
}

void ASideScroller1GameMode::BeginPlay()
{
	Super::BeginPlay();
	// Setup inventory HUD
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			RefreshHearts();
		}
	}

	// Enable mouse control
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	// Initialize GrabbedItem to null
	GrabbedItem = nullptr;

	// Initial score is 0
	Score = 0;
}

void ASideScroller1GameMode::Tick(float DeltaSeconds)
{
	
}

TArray<AItem*> ASideScroller1GameMode::GetInventory()
{
	return Inventory;
}

void ASideScroller1GameMode::AddToInventory(AItem *AnItem)
{
	Inventory.Add(AnItem);
	RefreshInventory();
}

void ASideScroller1GameMode::OnInventorySlotPressed(int32 Slot)
{
	if (!GrabbedItem)
	{
		GrabbedItem = Inventory[Slot];
		GrabbedItem->Grab();
	}
}

void ASideScroller1GameMode::OnInventorySlotReleased(AItem *Item)
{
	GrabbedItem = nullptr;
	Inventory.Remove(Item);
	RefreshInventory();
}

bool ASideScroller1GameMode::HasGrabbedItem()
{
	return (GrabbedItem)? true: false;
}

void ASideScroller1GameMode::AddToScore(int32 Score)
{
	this->Score += Score;
}