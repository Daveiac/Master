// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller1Character.h"
#include "Bait.h"
#include "Enemy.h"

AEnemy::AEnemy()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/2DSideScroller/Sprites/Character/Enemy1/SpiderWalk.SpiderWalk"))
			, IdleAnimationAsset(TEXT("/Game/2DSideScroller/Sprites/Character/Enemy1/SpiderIdle.SpiderIdle"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	WalkAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	GetSprite()->SetFlipbook(IdleAnimation);

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Set the detection sphere
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->AttachTo(GetCapsuleComponent());
	DetectionSphere->SetSphereRadius(250.0f);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// Prevent all automatic rotation behavior on the character
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 5.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1800.f;
	GetCharacterMovement()->GroundFriction = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(45.0f);

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	OnActorHit.AddDynamic(this, &AEnemy::OnHit);

	bIsBaitable = false;
	CurrentBait = nullptr;
}

void AEnemy::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? WalkAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (bIsBaitable)
	{	// Move towards closest bait
		if (!CurrentBait)
			CurrentBait = GetClosestBait();
		if (CurrentBait)
		{
			if (GetCapsuleComponent()->IsOverlappingActor(CurrentBait))
			{
				if (!GetWorldTimerManager().IsTimerActive(EatingTimer))
					GetWorldTimerManager().SetTimer(EatingTimer, this, &AEnemy::EatBait, 1.0f, true, 0);  // TODO: Eating rate
			}
			else
			{
				float BaitX = CurrentBait->GetActorLocation().X;
				float ThisX = GetActorLocation().X;
				MoveRight((ThisX < BaitX) - (BaitX < ThisX));
			}
		}
	}
	if (!CurrentBait && DetectionSphere->IsOverlappingActor(Character))
	{	// Move towards the player
		float CharX = Character->GetActorLocation().X;
		float ThisX = GetActorLocation().X;
		MoveRight((ThisX < CharX) - (CharX < ThisX));
	}
	UpdateCharacter();
}

ABait* AEnemy::GetClosestBait()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	TArray<AActor*> AllBait;
	ABait *Bait = nullptr;
	DetectionSphere->GetOverlappingActors(AllBait, ABait::StaticClass());
	if (AllBait.Num())
	{	// Get closest bait
		float XDistance = DetectionSphere->GetScaledSphereRadius();
		for (int i = 0; i < AllBait.Num(); i++)
		{  // TODO: Fix casting
			float Diff = FMath::Abs((AllBait[i]->GetActorLocation().X - Character->GetActorLocation().X));
			if (Diff < XDistance && (((ABait*)AllBait[i])->bIsEdible))
			{
				Bait = (ABait*)AllBait[i];
				XDistance = Diff;
			}
		}
	}
	return Bait;
}

void AEnemy::EatBait()
{
	CurrentBait->BeEaten(1);  // TODO: Eating rate
	if (!CurrentBait->bIsEdible)
	{	// The bait is no longer edible, stop the timer
		CurrentBait->Destroy();
		CurrentBait = nullptr;
		GetWorldTimerManager().ClearTimer(EatingTimer);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &AEnemy::MoveRight);
}

void AEnemy::MoveRight(float Value)
{
	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AEnemy::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	float TravelDirection = GetVelocity().X;
	// Set the rotation so that the character faces his direction of travel.
	if (TravelDirection < 0.0f)
	{
		SetActorRotation(FQuat(0, 0, 0, 0));
	}
	else if (TravelDirection > 0.0f)
	{
		SetActorRotation(FQuat(0, 0, 180, 0));
	}
}

void AEnemy::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		// Check if the OtherActor is a Character
		if (ASideScroller1Character *Character = Cast<ASideScroller1Character>(OtherActor))
		{
			Character->TakeDamage(1);
		}
	}
}



