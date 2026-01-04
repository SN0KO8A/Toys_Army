// Fill out your copyright notice in the Description page of Project Settings.


#include "TAVehicle_Base.h"

#include "VehicleEngine/TAVehicleEngine_Base.h"

ATAVehicle_Base::ATAVehicle_Base()
{
	VehicleStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("VehicleMesh");
	VehicleStaticMeshComponent->SetSimulatePhysics(true);

	SetRootComponent(VehicleStaticMeshComponent);
}

void ATAVehicle_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (VehicleEngine)
	{
		VehicleEngine->EngineTick(DeltaTime);
	}
}
