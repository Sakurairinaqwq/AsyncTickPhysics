#include "AsyncTickCallback.h"

#include "AsyncTickPawn.h"

void FAsyncPhysicsCallback::OnPreSimulate_Internal()
{
	const FAsyncPhysicsInput* Input = GetConsumerInput_Internal();

	if(Input == nullptr || Input->Pawns.IsEmpty())
		return;

	const float DeltaTime = GetDeltaTime_Internal();

	for(const TWeakObjectPtr<AAsyncTickPawn>& Pawn : Input->Pawns)
	{
		AAsyncTickPawn* ResolvedPawn = Pawn.Get();

		if(ResolvedPawn == nullptr)
		{
			continue;
		}

		ResolvedPawn->NativeAsyncTick(DeltaTime);
	}
}

FName FAsyncPhysicsCallback::GetFNameForStatId() const
{
	static const FName StatName(TEXT("FAsyncPhysicsCallback"));
	return StatName;
}
