// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER1_API AEnemy : public APaperCharacter
{
	GENERATED_BODY()

	AEnemy();

	UPROPERTY(EditAnywhere, Category = "Bait")
	bool bIsBaitable;
	UTextRenderComponent* TextComponent;
	class ABait *CurrentBait;
	FTimerHandle EatingTimer;
	virtual void Tick(float DeltaSeconds) override;
	ABait* GetClosestBait();
	void EatBait();

protected:
	// The animation to play while walking around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// Detection sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class USphereComponent* DetectionSphere;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
