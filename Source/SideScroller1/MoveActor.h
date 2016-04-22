// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MoveActor.generated.h"

UCLASS()
class SIDESCROLLER1_API AMoveActor : public AActor
{
	GENERATED_BODY()

class UPSMoveComponent *PSMoveComponent;

UPROPERTY(EditAnywhere, Category = "MotionController")
class UMotionControllerComponent *MotionController;

public:	
	// Sets default values for this actor's properties
	AMoveActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
