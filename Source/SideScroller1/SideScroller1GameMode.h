// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/GameMode.h"
#include "Item.h"
#include "SideScroller1GameMode.generated.h"

// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of SideScroller1Character

UCLASS(minimalapi)
class ASideScroller1GameMode : public AGameMode
{
	GENERATED_BODY()

UPROPERTY(VisibleAnywhere, Category = "Inventory")
TArray<AItem*> Inventory;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	ASideScroller1GameMode();
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<AItem*> GetInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddToInventory(AItem *AnItem);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void RefreshInventory();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void RefreshHearts();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnInventorySlotPressed(int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnInventorySlotReleased(AItem *Item);

	void AddToScore(int32 Score);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	class UUserWidget* CurrentWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	AItem *GrabbedItem;
};
