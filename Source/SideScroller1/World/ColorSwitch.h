// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Switch.h"
#include "ColorSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER1_API AColorSwitch : public ASwitch
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "ColorBox")
	int32 Channel;
	virtual void Activate() override;
	virtual void Deactivate() override;
	void Toggle();
};
