// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Item.generated.h"

UCLASS()
class SIDESCROLLER1_API AItem : public AActor
{
	GENERATED_BODY()

protected:
	float Counter;

	// Paper sprite to represent the visual item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UPaperSpriteComponent* PaperSpriteComponent;
	// The sprite that is shown before the item is picked up
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UPaperSprite* PickupSprite;
	// The sprite that is shown while grabing and after placement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UPaperSprite* PlacementSprite;
	// The inventory icon of this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* InventoryIcon;

	virtual void LoadAsstets();
	virtual void SetOpacity(float Opacity);

public:
	// True when the item can be picked up, false when it can not TODO: Change name
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bIsActive;
	// True when this item is grabbed by p2
	UPROPERTY(VisibleAnywhere, Category = "Item")
	bool bIsGrabbed;
	// True when p2 can click the item to pick it up.
	UPROPERTY(EditAnywhere, Category = "Item")
	bool bIsClickable;

	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Return whether the item is active or not
	UFUNCTION(BlueprintPure, Category = "Item")
	bool IsActive();

	// Change the items active state
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetActive(bool NewItemState);

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor);

	UFUNCTION()
	void DoOnClicked();

	// Pick up the item and add it to the inventory
	virtual void PickUp();
	// Grab the item 
	virtual void Grab();
	// Release the item
	virtual bool Release();
	// Get the position of the mouse
	FVector GetMouseWorldPos();
};
