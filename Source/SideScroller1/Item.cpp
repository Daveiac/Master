// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "SideScroller1Character.h"
#include "SideScroller1GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"


void AItem::PickUp()
{
	// Remove the Item from the world and add it to the inventory
	bIsActive = false;
	PaperSpriteComponent->SetVisibility(false);
	PaperSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (PlacementSprite)
		PaperSpriteComponent->SetSprite(PlacementSprite);
	ASideScroller1GameMode *GameMode = Cast<ASideScroller1GameMode>(UGameplayStatics::GetGameMode(this));
	GameMode->AddToInventory(this);
}

void AItem::Grab()
{
	bIsGrabbed = true;
	PaperSpriteComponent->SetVisibility(true);
	PaperSpriteComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

bool AItem::Release()
{
	bIsGrabbed = false;
	PaperSpriteComponent->SetSimulatePhysics(true);
	PaperSpriteComponent->SetConstraintMode(EDOFMode::XZPlane);
	PaperSpriteComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PaperSpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	return true; // TODO: Check for valid placement
}

void AItem::LoadAsstets()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> PickupSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> PlacementSprite;
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> InventoryIcon;
		FConstructorStatics()
			: PickupSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/grassBlock_Sprite.grassBlock_Sprite'"))
			, PlacementSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/Items/grassBlock_Sprite.grassBlock_Sprite'"))
			, InventoryIcon(TEXT("Texture2D'/Game/2DSideScroller/Sprites/Items/grassBlock.grassBlock'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	PickupSprite = ConstructorStatics.PickupSprite.Get();
	PlacementSprite = ConstructorStatics.PlacementSprite.Get();
	InventoryIcon = ConstructorStatics.InventoryIcon.Get();
	PaperSpriteComponent->SetSprite(PickupSprite);
}

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creat PaperSprite component
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	RootComponent = PaperSpriteComponent;
	PaperSpriteComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	PaperSpriteComponent->SetConstraintMode(EDOFMode::XZPlane);
	
	LoadAsstets();

	// Items start as active and not grabbed
	bIsActive = true;
	bIsGrabbed = false;
	// initialize counter
	Counter = 0.0f;

	OnActorBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	OnClicked.AddDynamic(this, &AItem::DoOnClicked);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	PaperSpriteComponent->SetSprite(PickupSprite);
	if (!bIsActive)
	{
		Grab();
		Release();
	}
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
	else if (bIsGrabbed)
	{
		FVector MousePos = GetMouseWorldPos();
		PaperSpriteComponent->SetWorldLocation(MousePos);
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
			PickUp();
		}
	}
}

void AItem::DoOnClicked()
{
	if (bIsGrabbed)
	{
		if (Release())
		{
			Cast<ASideScroller1GameMode>(UGameplayStatics::GetGameMode(GetWorld()))->
				OnInventorySlotReleased(this);
		}
	}
}

FVector AItem::GetMouseWorldPos()
{
	FVector WorldPos(0, 0, 0);
	FVector Direction(0, 0, 0);
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->GetMousePosition(WorldPos.X, WorldPos.Y);
	PlayerController->DeprojectMousePositionToWorld(WorldPos, Direction);
	WorldPos.Y = 0;
	return WorldPos;
}
