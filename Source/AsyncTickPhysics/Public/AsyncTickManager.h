#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "AsyncTickCallback.h"
#include "AsyncTickPawn.h"

class ASYNCTICKPHYSICS_API FAsyncTickManager
{
public:
	FAsyncTickManager(FPhysScene* PhysScene);
	~FAsyncTickManager();
	
	static void OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues);
	static void OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResource);

	FPhysScene_Chaos& GetScene() const { return Scene; }

	static FAsyncTickManager* GetPhysicsManagerFromScene(FPhysScene* PhysScene);

	void DetachFromPhysScene(FPhysScene* PhysScene);
	
	void AddPawn(TWeakObjectPtr<AAsyncTickPawn> Pawn);
	void RemovePawn(TWeakObjectPtr<AAsyncTickPawn> Pawn);

	void RegisterCallbacks();
	void UnregisterCallbacks();

	void ScenePreTick(FPhysScene* PhysScene, float DeltaTime);

private:
	static void RegisterWorldDelegates();
	static void UnregisterWorldDelegates();

	void CompactPawns();

	static TMap<FPhysScene*, FAsyncTickManager*> SceneToPhysicsManagerMap;
	static int32 ActiveManagerCount;
	static bool bWorldDelegatesRegistered;

	TArray<TWeakObjectPtr<AAsyncTickPawn>> Pawns;
	
	FDelegateHandle OnPhysScenePreTickHandle;

	static FDelegateHandle OnPostWorldInitializationHandle;
	static FDelegateHandle OnWorldCleanupHandle;
	
	FPhysScene_Chaos& Scene;
	
	FAsyncPhysicsCallback* AsyncObject;
	bool bCallbacksRegistered;
};
