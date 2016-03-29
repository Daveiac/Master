// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "SideScroller1Character.h"
#include "SideScroller1GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Items start as active and not grabbed
	bIsActive = true;
	bIsGrabbed = false;

	// Creat PaperSprite component
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	RootComponent = PaperSpriteComponent;
	PaperSpriteComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	

	// initialize counter
	Counter = 0.0f;
	
	OnActorBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	OnClicked.AddDynamic(this, &AItem::DoOnClicked);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	PaperSpriteComponent->SetSprite(PaperSprite);
}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bIsActive)
	{
		// Scale between 0.9 and 1.1
		float Scale = (sin(Counter) * 0.1f) + 1.0f;
		FVector Scale3D(Scale, 1, Scale);
		RootComponent->SetRelativeScale3D(Scale3D);
		Counter += DeltaTime * 5;
		if (Counter > 2 * PI) Counter = 0;
	}
}

bool AItem::IsActive()
{
	return bIsActive;
}

void AItem::SetActive(bool NewItemState)
{
	bIsActive = NewItemState;
}

void AItem::OnOverlapBegin(AActor *OtherActor)
{
	if (bIsActive && OtherActor != nullptr && OtherActor != this)
	{
		// Check if the OtherActor is a Character
		if (ASideScroller1Character *Character = Cast<ASideScroller1Character>(OtherActor))
		{
			ASideScroller1GameMode *GameMode = (ASideScroller1GameMode *)
				UGameplayStatics::GetGameMode(GetWorld());
			// Remove the Item from the world and add it to the inventory
			bIsActive = false;
			PaperSpriteComponent->SetVisibility(false);
			PaperSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GameMode->AddToInventory(this);
		}
	}
}

void AItem::DoOnClicked()
{
	if (bIsGrabbed)
	{
		Cast<ASideScroller1GameMode>(UGameplayStatics::GetGameMode(GetWorld()))->
			OnInventorySlotReleased(this);
	}
}