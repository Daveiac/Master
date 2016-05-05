// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Hand.generated.h"

UCLASS()
class SIDESCROLLER1_API AHand : public APawn
{
	GENERATED_BODY()

	class UPaperSpriteComponent *SpriteComponent;
	class UPaperSprite *PointSprite;
	class UPaperSprite *HoverSprite;
	class UPaperSprite *GrabSprite;
	class UPaperSprite *ThumbsUpSprite;
	class UPaperSprite *ThumbsDownSprite;
	FVector CurrentVelocity;
	UPROPERTY(EditAnywhere, Category = "Hand")
	float SpeedX = 550; // Vertical speed of the hand in unreal units pr second
	UPROPERTY(EditAnywhere, Category = "Hand")
	float SpeedZ = 550; // Horizontal speed of the hand in unreal units pr second
	class AItem *GrabbedItem;
	void LoadAssets();

public:
	// Sets default values for this pawn's properties
	AHand();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor *OtherActor) override;
	void UpdateOverlappingActors();

	void Activate();
	void CancelCurrentAction();
	void MoveRight(float Value);
	void MoveUp(float Value);
};
