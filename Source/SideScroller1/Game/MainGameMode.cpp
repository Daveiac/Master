// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideScroller1.h"
#include "MainGameMode.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"

AMainGameMode::AMainGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = AMainCharacter::StaticClass();

	// Set Tickable
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay()
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

void AMainGameMode::Tick(float DeltaSeconds)
{
	
}

TArray<AItem*> AMainGameMode::GetInventory()
{
	return Inventory;
}

void AMainGameMode::AddToInventory(AItem *AnItem)
{
	Inventory.Add(AnItem);
	RefreshInventory();
}

void AMainGameMode::OnInventorySlotPressed(int32 Slot)
{
	if (!GrabbedItem)
	{
		GrabbedItem = Inventory[Slot];
		GrabbedItem->Grab();
	}
}

void AMainGameMode::OnInventorySlotReleased(AItem *Item)
{
	GrabbedItem = nullptr;
	Inventory.Remove(Item);
	RefreshInventory();
}

bool AMainGameMode::HasGrabbedItem()
{
	return (GrabbedItem)? true: false;
}

void AMainGameMode::AddToScore(int32 Score)
{
	this->Score += Score;
}