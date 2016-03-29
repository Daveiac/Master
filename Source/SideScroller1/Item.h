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

float Counter;
protected:
	// True when the item can be used, false when it is not
	bool bIsActive;

public:
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Paper sprite to represent the visual item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UPaperSpriteComponent* PaperSpriteComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UPaperSprite* PaperSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* InventoryIcon;

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

	UPROPERTY(VisibleAnywhere, Category = "Item")
	bool bIsGrabbed;

};
