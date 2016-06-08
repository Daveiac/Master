// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideScroller1.h"
#include "MainCharacter.h"
#include "MainGameMode.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"



DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);
//////////////////////////////////////////////////////////////////////////
// AMainCharacter

AMainCharacter::AMainCharacter()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<USoundWave> DamageCharacterSound;
		ConstructorHelpers::FObjectFinderOptional<USoundCue> FootstepsSound;
		ConstructorHelpers::FObjectFinderOptional<USoundWave> DeathSound;
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/RunningAnimation.RunningAnimation"))
			, IdleAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/IdleAnimation.IdleAnimation"))
			, DamageCharacterSound(TEXT("SoundWave'/Game/2DSideScroller/Sound/SFX/TakeDamage.TakeDamage'"))
			, FootstepsSound(TEXT("SoundCue'/Game/2DSideScroller/Sound/SFX/Footstep_Cue.Footstep_Cue'"))
			, DeathSound(TEXT("SoundWave'/Game/2DSideScroller/Sound/SFX/death.death'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	GetSprite()->SetFlipbook(IdleAnimation);

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 5.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1800.f;
	GetCharacterMovement()->GroundFriction = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(45.0f);

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
// 	TextComponent->AttachTo(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Setup health
	Health = 10;
	
	// Create Sound Components
	SoundDamageTaken = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("DamageCharacter"));
	SoundDamageTaken->SetSound(ConstructorStatics.DamageCharacterSound.Get());
	SoundDamageTaken->bAutoActivate = false;

	SoundFootsteps = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Footsteps"));
	SoundFootsteps->SetSound(ConstructorStatics.FootstepsSound.Get());
	SoundFootsteps->bAutoActivate = false;
	SoundFootsteps->AttachTo(RootComponent);

	SoundDeath = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Death"));
	SoundDeath->SetSound(ConstructorStatics.DeathSound.Get());
	SoundDeath->bAutoActivate = false;
	SoundDeath->bStopWhenOwnerDestroyed = false;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AMainCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AMainCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	InputComponent->BindTouch(IE_Pressed, this, &AMainCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AMainCharacter::TouchStopped);
}

void AMainCharacter::MoveRight(float Value)
{
	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AMainCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AMainCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AMainCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		else if (TravelDirection > 0.0f)
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		
		// Play footsteps sound
		if (TravelDirection != 0 && GetCharacterMovement()->IsMovingOnGround() && !SoundFootsteps->IsPlaying())
			SoundFootsteps->Play();
		else if ((TravelDirection == 0 || !GetCharacterMovement()->IsMovingOnGround()) && SoundFootsteps->IsPlaying())
			SoundFootsteps->Stop();
	}
}

void AMainCharacter::DamageCharacter(int32 Damage)
{
	if (bIsInvincible) return;
	Health -= Damage;
	if (Health <= 0)
	{
		return Kill();
	}
	FTimerHandle InvincibilityTimer;
	bIsInvincible = true;
	GetWorldTimerManager().SetTimer(InvincibilityTimer, this, &AMainCharacter::SetNotInvincible, 0.5f); // TODO variable invincibilitytimer

	if (AMainGameMode *GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->RefreshHearts();
	}
	SoundDamageTaken->Play();
}

int32 AMainCharacter::GetHealth()
{
	return Health;
}

void AMainCharacter::SetNotInvincible()
{
	bIsInvincible = false;
}

void AMainCharacter::Kill()
{
	SoundDeath->Play();
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FTimerHandle DeathTimer;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AMainCharacter::RestartLevel, 0.45);
}

void AMainCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())));
}
