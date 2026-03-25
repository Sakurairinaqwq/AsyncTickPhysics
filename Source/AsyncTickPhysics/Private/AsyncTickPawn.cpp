#include "AsyncTickPawn.h"

#include "AsyncTickManager.h"

AAsyncTickPawn::AAsyncTickPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAsyncTickPawn::NativeAsyncTick(float DeltaTime)
{
	AsyncTick(DeltaTime);
}

void AAsyncTickPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FAsyncTickManager* AsyncTickManager = World ? FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()) : nullptr;

	if(ensureMsgf(AsyncTickManager != nullptr, TEXT("AsyncTickPhysics could not find a manager for world '%s'."), World ? *World->GetName() : TEXT("None")))
	{
		AsyncTickManager->AddPawn(this);
	}
}

void AAsyncTickPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UWorld* World = GetWorld();
	FAsyncTickManager* AsyncTickManager = World ? FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()) : nullptr;

	if(AsyncTickManager != nullptr)
	{
		AsyncTickManager->RemovePawn(this);
	}
}
