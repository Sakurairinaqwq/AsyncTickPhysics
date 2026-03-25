#pragma once

#include "CoreMinimal.h"
#include "AsyncTickPawn.h"
#include "PhysicsPublic.h"
#include "Chaos/SimCallbackInput.h"
#include "Chaos/SimCallbackObject.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "PhysicsProxy/SingleParticlePhysicsProxyFwd.h"

// Data consumed by the custom solver callback before each simulation step.
struct FAsyncPhysicsInput : public Chaos::FSimCallbackInput
{
	TArray<TWeakObjectPtr<AAsyncTickPawn>> Pawns;

	// Required method
	void Reset()
	{
		Pawns.Reset();
	}
};

// Output produced by the custom solver callback.
struct FAsyncPhysicsOutput : public Chaos::FSimCallbackOutput
{
	// Required method
	void Reset()
	{
	}
};

class FAsyncPhysicsCallback : public Chaos::TSimCallbackObject<
	FAsyncPhysicsInput,
	FAsyncPhysicsOutput,
	Chaos::ESimCallbackOptions::Presimulate | Chaos::ESimCallbackOptions::RunOnFrozenGameThread>
{
public:
	virtual FName GetFNameForStatId() const override;

private:
	// Keep using a custom Chaos callback, but execute on the frozen game thread so Blueprint access remains valid.
	virtual void OnPreSimulate_Internal() override;
};
