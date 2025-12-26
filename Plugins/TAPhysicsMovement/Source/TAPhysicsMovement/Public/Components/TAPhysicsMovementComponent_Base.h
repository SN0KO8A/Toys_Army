// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Physics/NetworkPhysicsComponent.h"
#include "TADeferredForcesLoader.h"

#include "TAPhysicsMovementComponent_Base.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class TAPHYSICSMOVEMENT_API UTAPhysicsMovementComponent_Base : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	UTAPhysicsMovementComponent_Base(const FObjectInitializer& ObjectInitializer);

	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;

	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;

	bool IsUsingNetworkPhysicsPrediction() const { return bIsUsingNetworkPhysicsPrediction; }

	UNetworkPhysicsComponent* GetNetworkPhysicsComponent() const { return NetworkPhysicsComponent; }

	FBodyInstance* GetBodyInstance() const;

	UFUNCTION(BlueprintCallable, Category = "TAPhysicsMovement")
	void AddForce(const FVector& Force, bool bIsAccelerationChange = false);

	UFUNCTION(BlueprintCallable, Category = "TAPhysicsMovement")
	void AddForceAtPosition(const FVector& Force, const FVector& Position);

	UFUNCTION(BlueprintCallable, Category = "TAPhysicsMovement")
	void AddTorque(const FVector& Torque, bool bIsAccelerationChange = false);

	UFUNCTION(BlueprintCallable, Category = "TAPhysicsMovement")
	void AddImpulse(const FVector& Impulse, bool bIsVelocityChange = false);

	UFUNCTION(BlueprintCallable, Category = "TAPhysicsMovement")
	void AddImpulseAtPosition(const FVector& Impulse, const FVector& Position);

	UFUNCTION(BlueprintCallable, Category = "TAPhysicsMovement")
	void ApplyForces();

	UFUNCTION(BlueprintPure, Category = "TAPhysicsMovement")
	bool IsApplyingForces() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "TAPhysicsMovement")
	bool bDebugLogging = false;

	virtual void BeginPlay() override;

	virtual void OnCreatePhysicsState() override;

	virtual void OnDestroyPhysicsState() override;

	virtual bool ShouldCreatePhysicsState() const override;

	void ErrorMessage(FString Message, FColor Color, double seconds) const;

	virtual void InitializeNetworkPhysicsMovement() {};

	virtual void SimulatePhysicsTick(double DeltaTime, Chaos::FRigidBodyHandle_Internal* InRigidBodyHandle) {};

	class USkeletalMeshComponent* GetPawnSkeletalMeshComponent() const;

	class UStaticMeshComponent* GetPawnStaticMeshComponent() const;

	class UMeshComponent* GetPawnMeshComponent() const;

private:
	UPROPERTY()
	TObjectPtr<UNetworkPhysicsComponent> NetworkPhysicsComponent = nullptr;
	
	TObjectPtr<Chaos::FRigidBodyHandle_Internal> RigidBodyHandle = nullptr;
	FBodyInstance* BodyInstance = nullptr;

	FTADeferredForcesLoader DeferredForcesLoader;

	bool bIsUsingNetworkPhysicsPrediction = false;
};
