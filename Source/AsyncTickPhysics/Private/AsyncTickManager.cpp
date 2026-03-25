#include "AsyncTickManager.h"

#include "PBDRigidsSolver.h"
#include "Physics/Experimental/PhysScene_Chaos.h"

int32 FAsyncTickManager::ActiveManagerCount = 0;
bool FAsyncTickManager::bWorldDelegatesRegistered = false;

FDelegateHandle FAsyncTickManager::OnPostWorldInitializationHandle;
FDelegateHandle FAsyncTickManager::OnWorldCleanupHandle;

TMap<FPhysScene*, FAsyncTickManager*> FAsyncTickManager::SceneToPhysicsManagerMap;

FAsyncTickManager::FAsyncTickManager(FPhysScene* PhysScene)
	: Scene(*PhysScene)
	, AsyncObject(nullptr)
	, bCallbacksRegistered(false)
{
	check(PhysScene != nullptr);

	if(ActiveManagerCount++ == 0)
	{
		RegisterWorldDelegates();
	}

	ensureMsgf(FAsyncTickManager::SceneToPhysicsManagerMap.Find(PhysScene) == nullptr, TEXT("AsyncTickPhysics manager was registered twice for the same physics scene."));

	FAsyncTickManager::SceneToPhysicsManagerMap.Add(PhysScene, this);
}

FAsyncTickManager::~FAsyncTickManager()
{
	UnregisterCallbacks();
	Pawns.Reset();
	DetachFromPhysScene(&Scene);

	if(ActiveManagerCount > 0 && --ActiveManagerCount == 0)
	{
		UnregisterWorldDelegates();
	}
}

void FAsyncTickManager::OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues)
{
	if(InWorld == nullptr)
	{
		return;
	}

	FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(InWorld->GetPhysicsScene());

	if(AsyncTickManager != nullptr)
	{
		AsyncTickManager->RegisterCallbacks();
	}
}

void FAsyncTickManager::OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResource)
{
	if(InWorld == nullptr)
	{
		return;
	}

	FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(InWorld->GetPhysicsScene());

	if(AsyncTickManager != nullptr)
	{
		AsyncTickManager->UnregisterCallbacks();
	}
}

FAsyncTickManager* FAsyncTickManager::GetPhysicsManagerFromScene(FPhysScene* PhysScene)
{
	if(PhysScene == nullptr)
	{
		return nullptr;
	}

	FAsyncTickManager* Manager = nullptr;
	FAsyncTickManager** ManagerPtr = FAsyncTickManager::SceneToPhysicsManagerMap.Find(PhysScene);

	if(ManagerPtr != nullptr)
	{
		Manager = *ManagerPtr;
	}

	return Manager;
}

void FAsyncTickManager::DetachFromPhysScene(FPhysScene* PhysScene)
{
	FAsyncTickManager::SceneToPhysicsManagerMap.Remove(PhysScene);
}

void FAsyncTickManager::AddPawn(TWeakObjectPtr<AAsyncTickPawn> Pawn)
{
	if(Pawn.IsValid())
	{
		CompactPawns();
		Pawns.AddUnique(Pawn);
	}
}

void FAsyncTickManager::RemovePawn(TWeakObjectPtr<AAsyncTickPawn> Pawn)
{
	Pawns.RemoveAllSwap(
		[Pawn](const TWeakObjectPtr<AAsyncTickPawn>& RegisteredPawn)
		{
			return !RegisteredPawn.IsValid() || RegisteredPawn == Pawn;
		},
		EAllowShrinking::No);
}

void FAsyncTickManager::RegisterCallbacks()
{
	if(bCallbacksRegistered)
	{
		return;
	}

	if(Chaos::FPhysicsSolver* Solver = Scene.GetSolver())
	{
		AsyncObject = Solver->CreateAndRegisterSimCallbackObject_External<FAsyncPhysicsCallback>();
	}

	if(AsyncObject == nullptr)
	{
		return;
	}

	OnPhysScenePreTickHandle = Scene.OnPhysScenePreTick.AddRaw(this, &FAsyncTickManager::ScenePreTick);
	bCallbacksRegistered = true;
}

void FAsyncTickManager::UnregisterCallbacks()
{
	if(OnPhysScenePreTickHandle.IsValid())
	{
		Scene.OnPhysScenePreTick.Remove(OnPhysScenePreTickHandle);
		OnPhysScenePreTickHandle = FDelegateHandle();
	}

	if(AsyncObject != nullptr)
	{
		if(Chaos::FPhysicsSolver* Solver = Scene.GetSolver())
		{
			Solver->UnregisterAndFreeSimCallbackObject_External(AsyncObject);
		}

		AsyncObject = nullptr;
	}

	bCallbacksRegistered = false;
}

void FAsyncTickManager::ScenePreTick(FPhysScene* PhysScene, float DeltaTime)
{
	(void)PhysScene;
	(void)DeltaTime;

	if(AsyncObject == nullptr)
		return;

	FAsyncPhysicsInput* AsyncInput = AsyncObject->GetProducerInputData_External();

	if(AsyncInput == nullptr)
		return;

	AsyncInput->Reset();
	CompactPawns();

	AsyncInput->Pawns.Reserve(Pawns.Num());
	AsyncInput->Pawns.Append(Pawns);
}

void FAsyncTickManager::RegisterWorldDelegates()
{
	if(bWorldDelegatesRegistered)
	{
		return;
	}

	OnPostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddStatic(&FAsyncTickManager::OnPostWorldInitialization);
	OnWorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddStatic(&FAsyncTickManager::OnWorldCleanup);
	bWorldDelegatesRegistered = true;
}

void FAsyncTickManager::UnregisterWorldDelegates()
{
	if(!bWorldDelegatesRegistered)
	{
		return;
	}

	FWorldDelegates::OnPostWorldInitialization.Remove(OnPostWorldInitializationHandle);
	FWorldDelegates::OnWorldCleanup.Remove(OnWorldCleanupHandle);
	OnPostWorldInitializationHandle = FDelegateHandle();
	OnWorldCleanupHandle = FDelegateHandle();
	bWorldDelegatesRegistered = false;
}

void FAsyncTickManager::CompactPawns()
{
	Pawns.RemoveAllSwap(
		[](const TWeakObjectPtr<AAsyncTickPawn>& Pawn)
		{
			return !Pawn.IsValid();
		},
		EAllowShrinking::No);
}
