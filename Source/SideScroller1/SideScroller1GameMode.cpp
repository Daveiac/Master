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
	if (GrabbedItem)
	{
		FVector WorldPos = GetMouseWorldPos();
		GrabbedItem->PaperSpriteComponent->SetWorldLocation(WorldPos);
	}
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
	FVector WorldPos = GetMouseWorldPos();
	FRotator WorldRot(0.0f, 0.0f, 0.0f);
	GrabbedItem = Inventory[Slot];
	GrabbedItem->bIsGrabbed = true;
	UPaperSpriteComponent *Sprite = GrabbedItem->PaperSpriteComponent;
	Sprite->SetWorldLocation(WorldPos);
	Sprite->SetVisibility(true);
	Sprite->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASideScroller1GameMode::OnInventorySlotReleased(AItem *Item)
{
	GrabbedItem->bIsGrabbed = false;
	UPaperSpriteComponent *Sprite = GrabbedItem->PaperSpriteComponent;
	Sprite->SetSimulatePhysics(true);
	Sprite->SetConstraintMode(EDOFMode::XZPlane);
	Sprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GrabbedItem = nullptr;
	Inventory.Remove(Item);
	RefreshInventory();
}

FVector ASideScroller1GameMode::GetMouseWorldPos()
{
	FVector WorldPos(0, 0, 0);
	FVector Direction(0, 0, 0);
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->GetMousePosition(WorldPos.X, WorldPos.Y);
	PlayerController->DeprojectMousePositionToWorld(WorldPos, Direction);
	WorldPos.Y = 0;
	return WorldPos;
}

void ASideScroller1GameMode::AddToScore(int32 Score)
{
	this->Score += Score;
}