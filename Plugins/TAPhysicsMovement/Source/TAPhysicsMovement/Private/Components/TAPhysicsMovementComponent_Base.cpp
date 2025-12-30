// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TAPhysicsMovementComponent_Base.h"

#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

UTAPhysicsMovementComponent_Base::UTAPhysicsMovementComponent_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	bIsUsingNetworkPhysicsPrediction = Chaos::FPhysicsSolverBase::IsNetworkPhysicsPredictionEnabled();

	if (bIsUsingNetworkPhysicsPrediction)
	{
		static const FName NetworkPhysicsComponentName = TEXT("TAPhysicsMovement_NetworkComponent");
		NetworkPhysicsComponent = CreateDefaultSubobject<UNetworkPhysicsComponent, UNetworkPhysicsComponent>(NetworkPhysicsComponentName);
		NetworkPhysicsComponent->SetNetAddressable();
		NetworkPhysicsComponent->SetIsReplicated(true);
	}
}


void UTAPhysicsMovementComponent_Base::BeginPlay()
{
	Super::BeginPlay();
}

void UTAPhysicsMovementComponent_Base::OnCreatePhysicsState()
{
	Super::OnCreatePhysicsState();

	if (UWorld* World = GetWorld())
	{
		if (World->IsGameWorld())
		{
			if (UpdatedComponent && UpdatedPrimitive)
			{
				SetAsyncPhysicsTickEnabled(true);
			}

			InitializeNetworkPhysicsMovement();
		}
	}

	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(UpdatedComponent))
	{
		BodyInstance = PrimitiveComponent->GetBodyInstance();

		if (BodyInstance)
		{
			if (const FPhysicsActorHandle PhysicsActorHandle = BodyInstance->GetPhysicsActorHandle())
			{
				RigidBodyHandle = PhysicsActorHandle->GetPhysicsThreadAPI();
			}
		}
	}
}

void UTAPhysicsMovementComponent_Base::OnDestroyPhysicsState()
{
	if (UpdatedComponent)
	{
		UpdatedComponent->RecreatePhysicsState();
	}

	if (NetworkPhysicsComponent)
	{
		NetworkPhysicsComponent->RemoveDataHistory();
	}

	SetAsyncPhysicsTickEnabled(false);
	BodyInstance = nullptr;

	Super::OnDestroyPhysicsState();
}

bool UTAPhysicsMovementComponent_Base::ShouldCreatePhysicsState() const
{
	if (!IsRegistered() || IsBeingDestroyed())
	{
		return false;
	}

	if (UWorld* World = GetWorld())
	{
		if (World->IsGameWorld())
		{
			if (FPhysScene* PhysScene = World->GetPhysicsScene())
			{
				if (UpdatedComponent && UpdatedPrimitive)
				{
					return true;
				}
			}

		}
	}

	return false;
}

void UTAPhysicsMovementComponent_Base::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	if (BodyInstance)
	{
		if (const FPhysicsActorHandle PhysicsActorHandle = BodyInstance->GetPhysicsActorHandle())
		{
			RigidBodyHandle = PhysicsActorHandle->GetPhysicsThreadAPI();
		}
	}

	if (!RigidBodyHandle)
	{
		return;
	}

	CurrentVelocity = RigidBodyHandle->GetV();
	CurrentAngularVelocityInRadians = RigidBodyHandle->GetW();

	if (UWorld* World = GetWorld())
	{
		SimulatePhysicsTick(DeltaTime, RigidBodyHandle);
		DeferredForcesLoader.Apply(RigidBodyHandle);
	}
}

FBodyInstance* UTAPhysicsMovementComponent_Base::GetBodyInstance() const
{
	return BodyInstance;
}

void UTAPhysicsMovementComponent_Base::AddForce(const FVector& Force, bool bIsMassIncluded)
{
	FTAForceData ForceData = FTAForceData(Force, bIsMassIncluded);
	DeferredForcesLoader.Add(ForceData);
}

void UTAPhysicsMovementComponent_Base::AddForceAtPosition(const FVector& Force, const FVector& Position)
{
	FTAForceAtPositionData ForceAtPositionData = FTAForceAtPositionData(Force, Position);
	DeferredForcesLoader.Add(ForceAtPositionData);
}

void UTAPhysicsMovementComponent_Base::AddTorque(const FVector& Torque, bool bIncludeObjectWorldInertia)
{
	FTATorqueData TorqueData = FTATorqueData(Torque, bIncludeObjectWorldInertia);
	DeferredForcesLoader.Add(TorqueData);
}

void UTAPhysicsMovementComponent_Base::AddImpulse(const FVector& Impulse, bool bIsMassIncluded)
{
	FTAImpulseData ImpulseData = FTAImpulseData(Impulse, bIsMassIncluded);
	DeferredForcesLoader.Add(ImpulseData);
}

void UTAPhysicsMovementComponent_Base::AddImpulseAtPosition(const FVector& Impulse, const FVector& Position)
{
	FTAImpulseAtPositionData ImpulseAtPositionData = FTAImpulseAtPositionData(Impulse, Position);
	DeferredForcesLoader.Add(ImpulseAtPositionData);
}

void UTAPhysicsMovementComponent_Base::ApplyForces()
{
	DeferredForcesLoader.Unlock();
}

bool UTAPhysicsMovementComponent_Base::IsApplyingForces() const
{
	return DeferredForcesLoader.IsLocked();
}

void UTAPhysicsMovementComponent_Base::ErrorMessage(FString Message, FColor Color, double seconds) const
{
	if (GEngine && bDebugLogging)
	{
		GEngine->AddOnScreenDebugMessage(-1, seconds, Color, Message, true);
	}
}

USkeletalMeshComponent* UTAPhysicsMovementComponent_Base::GetPawnSkeletalMeshComponent() const
{
	return Cast<USkeletalMeshComponent>(GetPawnOwner()->GetRootComponent());
}

UStaticMeshComponent* UTAPhysicsMovementComponent_Base::GetPawnStaticMeshComponent() const
{
	return Cast<UStaticMeshComponent>(GetPawnOwner()->GetRootComponent());
}

UMeshComponent* UTAPhysicsMovementComponent_Base::GetPawnMeshComponent() const
{
	return Cast<UMeshComponent>(GetPawnOwner()->GetRootComponent());
}

void UTAPhysicsMovementComponent_Base::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	PawnOwner = NewUpdatedComponent ? Cast<APawn>(NewUpdatedComponent->GetOwner()) : nullptr;
}