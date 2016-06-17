// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "MainCharacter.h"
#include "Item.h"
#include "Switch.h"
#include "Hand.h"


void AHand::LoadAssets()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> PointSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> HoverSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> GrabSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> ThumbsUpSprite;
		ConstructorHelpers::FObjectFinderOptional<UPaperSprite> ThumbsDownSprite;
		FConstructorStatics()
			: PointSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/HUD/hand_cursor_point_Sprite.hand_cursor_point_Sprite'"))
			, HoverSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/HUD/hand_cursor_hover_Sprite.hand_cursor_hover_Sprite'"))
			, GrabSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/HUD/hand_cursor_grab_Sprite.hand_cursor_grab_Sprite'"))
			, ThumbsUpSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/HUD/hand_cursor_thumbsup_Sprite.hand_cursor_thumbsup_Sprite'"))
			, ThumbsDownSprite(TEXT("PaperSprite'/Game/2DSideScroller/Sprites/HUD/hand_cursor_thumbsdown_Sprite.hand_cursor_thumbsdown_Sprite'"))
		{}
	};
	static FConstructorStatics ConstructorStatics;
	SpriteComponent = CreateAbstractDefaultSubobject<UPaperSpriteComponent>("SpriteComponent");
	RootComponent = SpriteComponent;
	PointSprite = ConstructorStatics.PointSprite.Get();
	HoverSprite = ConstructorStatics.HoverSprite.Get();
	GrabSprite = ConstructorStatics.GrabSprite.Get();
	ThumbsUpSprite = ConstructorStatics.ThumbsUpSprite.Get();
	ThumbsDownSprite = ConstructorStatics.ThumbsDownSprite.Get();

	SpriteComponent->SetSprite(PointSprite);
	SpriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Sets default values
AHand::AHand()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LoadAssets();
	AutoPossessPlayer = EAutoReceiveInput::Player1;
	CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);
	GrabbedItem = nullptr;
}

// Called when the game starts or when spawned
void AHand::BeginPlay()
{
	Super::BeginPlay();
	APlayerController *HandPlayer = UGameplayStatics::CreatePlayer(this, 1, true);
}

// Called every frame
void AHand::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Keep the hand in view of the Camera
	AMainCharacter *Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	float CamRadiusX = Character->GetSideViewCameraComponent()->OrthoWidth / 2;
	float CamRadiusZ = CamRadiusX / Character->GetSideViewCameraComponent()->AspectRatio;
	float CamZOffset = Character->GetCameraBoom()->SocketOffset.Z;
	float CharX = Character->GetActorLocation().X;
	float CharZ = Character->GetActorLocation().Z;

	float CameraMaxX = CharX + CamRadiusX - PointSprite->GetBakedTexture()->GetSizeX() / 2;
	float CameraMinX = CharX - CamRadiusX + PointSprite->GetBakedTexture()->GetSizeX() / 2;
	float CameraMaxZ = CharZ + CamRadiusZ + CamZOffset - PointSprite->GetBakedTexture()->GetSizeY() / 2;
	float CameraMinZ = CharZ - CamRadiusZ - CamZOffset + PointSprite->GetBakedTexture()->GetSizeY() / 2;

	float newX = GetActorLocation().X + (CurrentVelocity.X * DeltaTime);
	float newZ = GetActorLocation().Z + (CurrentVelocity.Z * DeltaTime);

	if (newX > CameraMaxX)
		newX = CameraMaxX;
	else if (newX < CameraMinX)
		newX = CameraMinX;

	if (newZ > CameraMaxZ)
		newZ = CameraMaxZ;
	else if (newZ < CameraMinZ)
		newZ = CameraMinZ;

	// Move the hand according to the current position determined by player input + camera
	SetActorLocation(FVector(newX, 1.0f, newZ));

	// Make the mouse follow the hand.
	APlayerController *Controller = UGameplayStatics::GetPlayerController(this, 0);
	FViewport *Viewport = Controller->GetLocalPlayer()->ViewportClient->Viewport;
	if (Viewport && Viewport->IsForegroundWindow())
	{
		FVector2D ScreenLocation;
		Controller->ProjectWorldLocationToScreen(GetActorLocation(), ScreenLocation);
		Viewport->SetMouse(ScreenLocation.X, ScreenLocation.Y);
	}

	// Detect when items are grabbed and released in order to change the sprite
	if (GrabbedItem)
	{
		if (GrabbedItem->bIsGrabbed && SpriteComponent->GetSprite() != GrabSprite)
			SpriteComponent->SetSprite(GrabSprite);
		else if (!GrabbedItem->bIsGrabbed && SpriteComponent->GetSprite() != HoverSprite)
			SpriteComponent->SetSprite(HoverSprite);
	}
}

// Called to bind functionality to input
void AHand::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// Bind input actions of the hand
	InputComponent->BindAction("Activate", IE_Pressed, this, &AHand::Activate);
	InputComponent->BindAction("Cancel", IE_Pressed, this, &AHand::CancelCurrentAction);
	// Bind input axes of the hand
	InputComponent->BindAxis("MoveRight", this, &AHand::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &AHand::MoveUp);
}


void AHand::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (AItem *Item = Cast<AItem>(OtherActor))
	{
		if (!Item->bIsActive)
		{
			if (Item->bIsGrabbed)
				SpriteComponent->SetSprite(GrabSprite);
			else if (Item->bIsClickable)
				SpriteComponent->SetSprite(HoverSprite);
			if (!GrabbedItem)
				GrabbedItem = Item;
		}
	}
	else if (Cast<ASwitch>(OtherActor))
	{
		SpriteComponent->SetSprite(HoverSprite);
	}
}

void AHand::NotifyActorEndOverlap(AActor * OtherActor)
{
	if (AItem *Item = Cast<AItem>(OtherActor))
	{
		SpriteComponent->SetSprite(PointSprite);
		if (Item == GrabbedItem)
			GrabbedItem = nullptr;
	}
	else if (Cast<ASwitch>(OtherActor))
		SpriteComponent->SetSprite(PointSprite);
}

void AHand::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate!"));
	// Simulate mouse press
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void AHand::CancelCurrentAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Cancel!"))
}

void AHand::MoveRight(float Value)
{
	CurrentVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * SpeedX;
}

void AHand::MoveUp(float Value)
{
	CurrentVelocity.Z = FMath::Clamp(Value, -1.0f, 1.0f) * SpeedZ;
}

