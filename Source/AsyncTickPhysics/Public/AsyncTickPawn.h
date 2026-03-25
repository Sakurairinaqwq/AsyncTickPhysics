#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AsyncTickPawn.generated.h"

UCLASS(BlueprintType)
class ASYNCTICKPHYSICS_API AAsyncTickPawn : public APawn
{
	GENERATED_BODY()

public:
	AAsyncTickPawn();

	// This tick is driven by the plugin's custom Chaos callback, not by UE's built-in Async Physics Tick registration.
	UFUNCTION(BlueprintImplementableEvent, Category = "AsyncTick")
		void AsyncTick(float DeltaTime);
	
	virtual void NativeAsyncTick(float DeltaTime);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
