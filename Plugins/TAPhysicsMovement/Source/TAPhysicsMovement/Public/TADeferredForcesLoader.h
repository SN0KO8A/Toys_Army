// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace Chaos
{
	class FRigidBodyHandle_Internal;
}

struct TAPHYSICSMOVEMENT_API FTAForceData
{

public:
	FTAForceData(const FVector& ForceInput, bool IncludeMass)
	{
		Force = ForceInput;
		bIncludeMass = IncludeMass;
	}
	~FTAForceData() {}

	FVector Force;
	bool bIncludeMass = false;
};

struct TAPHYSICSMOVEMENT_API FTAForceAtPositionData
{

public:
	FTAForceAtPositionData(const FVector& ForceInput, const FVector& PositionInput) 
	{
		Force = ForceInput;
		Position = PositionInput;
	}

	~FTAForceAtPositionData() {}

	FVector Force;
	FVector Position;
};

struct TAPHYSICSMOVEMENT_API FTATorqueData
{

public:
	FTATorqueData(const FVector& TorqueInput, bool IncludeWorldInertia)
	{
		Torque = TorqueInput;
		bIncludeWorldInertia = IncludeWorldInertia;
	}
	~FTATorqueData() {}

	FVector Torque;
	bool bIncludeWorldInertia = false;
};

struct TAPHYSICSMOVEMENT_API FTAImpulseData
{

public:
	FTAImpulseData(const FVector& ImpulseInput, bool IncludeMass)
	{
		Impulse = ImpulseInput;
		bIncludeMass = IncludeMass;
	}
	~FTAImpulseData() {}

	FVector Impulse;
	bool bIncludeMass = false;
};

struct TAPHYSICSMOVEMENT_API FTAImpulseAtPositionData
{

public:
	FTAImpulseAtPositionData(const FVector& ImpulseInput, const FVector& PositionInput)
	{
		Impulse = ImpulseInput;
		Position = PositionInput;
	}
	~FTAImpulseAtPositionData() {}

	FVector Impulse;
	FVector Position;
};

struct TAPHYSICSMOVEMENT_API FTADeferredForcesLoader
{

public:
	FTADeferredForcesLoader() {}
	~FTADeferredForcesLoader() {}

	void Add(const FTAForceData& ForceData) { ApplyForces.Add(ForceData); }
	void Add(const FTAForceAtPositionData& ForceAtPositionData) { ApplyForcesAtPosition.Add(ForceAtPositionData); }
	void Add(const FTATorqueData& TorqueData) { ApplyTorques.Add(TorqueData); }
	void Add(const FTAImpulseData& ImpulseData) { ApplyImpulses.Add(ImpulseData); }
	void Add(const FTAImpulseAtPositionData& ImpulseAtPositionData) { ApplyImpulsesAtPosition.Add(ImpulseAtPositionData); }

	void Apply(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle);

	void Reset();

	void Unlock();

	bool IsLocked() const;
private:
	bool bIsLocked = true;

	TArray<FTAForceData> ApplyForces;
	TArray<FTAForceAtPositionData> ApplyForcesAtPosition;
	TArray<FTATorqueData> ApplyTorques;
	TArray<FTAImpulseData> ApplyImpulses;
	TArray<FTAImpulseAtPositionData> ApplyImpulsesAtPosition;

	void AddForce(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAForceData& ForceData);
	void AddForceAtPosition(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAForceAtPositionData& ForceAtPositionData);
	void AddTorque(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTATorqueData& TorqueData);
	void AddImpulse(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAImpulseData& ImpulseData);
	void AddImpulseAtPosition(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAImpulseAtPositionData& ImpulseAtPositionData);
};
