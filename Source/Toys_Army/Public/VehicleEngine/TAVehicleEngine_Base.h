// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TAVehicleEngine_Base.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class TOYS_ARMY_API UTAVehicleEngine_Base : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void EngineTick(float DeltaTime) {};

	virtual void AddInputValue(float InputValue) {};
	virtual float GetEngineValue() { return EngineValue; }

protected:
	float EngineValue = 0.f;
};
