// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "MoveActor.h"


// Sets default values
AMoveActor::AMoveActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMoveActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMoveActor::Tick( float DeltaTime )
{
	//FPSMoveDataContext *DataContext;
	//FPSMoveWorker::GetSingletonInstance()->AcquirePSMove(0, DataContext);
	//if (DataContext->GetIsTracking()) 
	//{
	//	Position = DataContext->GetPosition();
	//}
}

