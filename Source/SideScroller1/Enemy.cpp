// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScroller1.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller1Character.h"
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

	float yaw = Controller->GetControlRotation().Yaw + 1;
	Controller->SetControlRotation(FRotator(0.0, yaw, 0.0f));

	if (DetectionSphere->IsOverlappingActor(Character))
	{
		// Move towards the player
		float CharX = Character->GetActorLocation().X;
		float ThisX = GetActorLocation().X;
		MoveRight((ThisX < CharX) - (CharX < ThisX));
	}

	UpdateCharacter();
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
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
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



