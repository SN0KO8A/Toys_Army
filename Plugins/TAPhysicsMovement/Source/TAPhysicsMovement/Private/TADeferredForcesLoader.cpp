

#include "TADeferredForcesLoader.h"

#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "Chaos/Particle/ParticleUtilities.h"
#include "Chaos/Utilities.h"

#pragma optimize( "", off )
void FTADeferredForcesLoader::Apply(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle)
{
	if (bIsLocked)
	{
		return;
	}

	bool BP_BreakPoint = ApplyForces.Num() > 0;

	for (const FTAForceData& CurrentForceData : ApplyForces)
	{
		AddForce(RigidBodyHandle, CurrentForceData);
	}
	for (const FTAForceAtPositionData& CurrentForceAtPositionData : ApplyForcesAtPosition)
	{
		AddForceAtPosition(RigidBodyHandle, CurrentForceAtPositionData);
	}
	for (const FTATorqueData& CurrentTorqueData : ApplyTorques)
	{
		AddTorque(RigidBodyHandle, CurrentTorqueData);
	}
	for (const FTAImpulseData& CurrentImpulseData : ApplyImpulses)
	{
		AddImpulse(RigidBodyHandle, CurrentImpulseData);
	}
	for (const FTAImpulseAtPositionData& CurrentImpulseAtPositionData : ApplyImpulsesAtPosition)
	{
		AddImpulseAtPosition(RigidBodyHandle, CurrentImpulseAtPositionData);
	}

	Reset();

	bIsLocked = true;
}
#pragma optimize( "", on )

void FTADeferredForcesLoader::Reset()
{
	ApplyForces.Empty();
	ApplyForcesAtPosition.Empty();
	ApplyTorques.Empty();
	ApplyImpulses.Empty();
	ApplyImpulsesAtPosition.Empty();
}

void FTADeferredForcesLoader::Unlock()
{
	bIsLocked = false;
}

bool FTADeferredForcesLoader::IsLocked() const
{
	return bIsLocked;
}

void FTADeferredForcesLoader::AddForce(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAForceData& ForceData)
{
	if (ensure(RigidBodyHandle))
	{
		if (ForceData.bIncludeMass)
		{
			const Chaos::FVec3 force = ForceData.Force * RigidBodyHandle->M();
			RigidBodyHandle->AddForce(force, true);
		}
		else
		{
			RigidBodyHandle->AddForce(ForceData.Force, true);
		}
	}
}

void FTADeferredForcesLoader::AddForceAtPosition(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAForceAtPositionData& ForceAtPositionData)
{
	if (ensure(RigidBodyHandle))
	{
		const Chaos::FVec3 WorldCenterOfMass = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidBodyHandle);
		const Chaos::FVec3 WorldTorque = Chaos::FVec3::CrossProduct(ForceAtPositionData.Position - WorldCenterOfMass, ForceAtPositionData.Force);
		RigidBodyHandle->AddForce(ForceAtPositionData.Force, true);
		RigidBodyHandle->AddTorque(WorldTorque, true);
	}
}

void FTADeferredForcesLoader::AddTorque(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTATorqueData& TorqueData)
{
	if (ensure(RigidBodyHandle))
	{
		if (TorqueData.bIncludeObjectWorldInertia)
		{
			Chaos::FVec3 torque = Chaos::FParticleUtilitiesXR::GetWorldInertia(RigidBodyHandle) * TorqueData.Torque;
			RigidBodyHandle->AddTorque(torque, true);
		}
		else
		{
			RigidBodyHandle->AddTorque(TorqueData.Torque, true);
		}
	}
}

void FTADeferredForcesLoader::AddImpulse(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAImpulseData& ImpulseData)
{
	if (ensure(RigidBodyHandle))
	{
		if (ImpulseData.bIncludeMass)
		{
			Chaos::FVec3 impulse = ImpulseData.Impulse * RigidBodyHandle->M();
			RigidBodyHandle->SetLinearImpulse(RigidBodyHandle->LinearImpulse() + impulse, true);
		}
		else
		{
			RigidBodyHandle->SetLinearImpulse(RigidBodyHandle->LinearImpulse() + ImpulseData.Impulse, true);
		}
	}
}

void FTADeferredForcesLoader::AddImpulseAtPosition(Chaos::FRigidBodyHandle_Internal* RigidBodyHandle, const FTAImpulseAtPositionData& ImpulseAtPositionData)
{
	if (ensure(RigidBodyHandle))
	{
		const Chaos::FVec3 WorldCenterOfMass = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidBodyHandle);
		const Chaos::FVec3 WorldAngularImpulse = Chaos::FVec3::CrossProduct(ImpulseAtPositionData.Position - WorldCenterOfMass, ImpulseAtPositionData.Impulse);
		RigidBodyHandle->SetLinearImpulse(RigidBodyHandle->LinearImpulse() + ImpulseAtPositionData.Impulse, true);
		RigidBodyHandle->SetAngularImpulse(RigidBodyHandle->AngularImpulse() + WorldAngularImpulse, true);
	}
}
