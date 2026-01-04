#include "CT_GameFlowSubsystem.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UCT_GameFlowSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[CT] GameFlowSubsystem Initialize"));
	CurrentState = ECT_GameFlowState::Boot;
}

void UCT_GameFlowSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("[CT] GameFlowSubsystem Deinitialize"));
	Super::Deinitialize();
}

void UCT_GameFlowSubsystem::SetState(ECT_GameFlowState NewState)
{
	if (CurrentState == NewState) return;

	const ECT_GameFlowState Old = CurrentState;
	CurrentState = NewState;

	UE_LOG(LogTemp, Log, TEXT("[CT] Flow State: %d -> %d"), (int32)Old, (int32)NewState);
	OnStateChanged.Broadcast(Old, NewState);
}

void UCT_GameFlowSubsystem::TogglePause()
{
	// Only allow pausing from InGame and Paused
	if (CurrentState == ECT_GameFlowState::InGame)
	{
		SetState(ECT_GameFlowState::Paused);
	}
	else if (CurrentState == ECT_GameFlowState::Paused)
	{
		SetState(ECT_GameFlowState::InGame);
	}
}