// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TAVehicle_Base.generated.h"

class UTAVehicleEngine_Base;

UCLASS(Abstract)
class TOYS_ARMY_API ATAVehicle_Base : public APawn
{
	GENERATED_BODY()

public:
	ATAVehicle_Base();

	virtual void Tick(float DeltaTime) override;

	virtual void Move(FVector2D MoveInput) {};
	virtual void Look(FVector2D LookInput) {};

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> VehicleStaticMeshComponent = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UTAVehicleEngine_Base> VehicleEngine = nullptr;
};
