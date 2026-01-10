#include "Subsystems/CT_NPCScheduleSubsystem.h"
#include "Subsystems/CT_TimeSubsystem.h"

void UCT_NPCScheduleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(UCT_TimeSubsystem::StaticClass());

	TimeSubsystem = GetGameInstance()->GetSubsystem<UCT_TimeSubsystem>();
	if (TimeSubsystem)
	{
		TimeSubsystem->OnTimeUpdated.AddUObject(this, &UCT_NPCScheduleSubsystem::HandleTimeUpdated);

		// Apply once at start so debug overlay isn't empty
		ApplyAll(TimeSubsystem->GetTimeBlock());
	}
}

void UCT_NPCScheduleSubsystem::Deinitialize()
{
	if (TimeSubsystem)
	{
		TimeSubsystem->OnTimeUpdated.RemoveAll(this);
	}
	Super::Deinitialize();
}

void UCT_NPCScheduleSubsystem::RegisterSchedule(const FCT_NPCSchedule& Schedule)
{
	// Replace if already exists
	for (FCT_NPCSchedule& S : Schedules)
	{
		if (S.NPCId == Schedule.NPCId)
		{
			S = Schedule;
			ApplyAll(TimeSubsystem ? TimeSubsystem->GetTimeBlock() : ECT_TimeBlock::Morning);
			return;
		}
	}
	Schedules.Add(Schedule);
	ApplyAll(TimeSubsystem ? TimeSubsystem->GetTimeBlock() : ECT_TimeBlock::Morning);
}

bool UCT_NPCScheduleSubsystem::UnregisterSchedule(FName NPCId)
{
	const int32 Removed = Schedules.RemoveAll([&](const FCT_NPCSchedule& S) { return S.NPCId == NPCId; });
	ResolvedStates.RemoveAll([&](const FCT_NPCScheduleResolvedState& R) { return R.NPCId == NPCId; });
	return Removed > 0;
}

void UCT_NPCScheduleSubsystem::ClearAllSchedules()
{
	Schedules.Reset();
	ResolvedStates.Reset();
}

TArray<FCT_NPCScheduleResolvedState> UCT_NPCScheduleSubsystem::GetAllResolvedStates() const
{
	return ResolvedStates;
}

void UCT_NPCScheduleSubsystem::HandleTimeUpdated(int32 /*NewDay*/, int32 /*NewMinutes*/, ECT_TimeBlock NewBlock)
{
	ApplyAll(NewBlock);
}

void UCT_NPCScheduleSubsystem::ApplyAll(ECT_TimeBlock Block)
{
	ResolvedStates.Reset();
	ResolvedStates.Reserve(Schedules.Num());

	for (const FCT_NPCSchedule& S : Schedules)
	{
		FCT_NPCScheduleResolvedState State;
		if (ResolveForBlock(S, Block, State))
		{
			ResolvedStates.Add(State);
			OnScheduleApplied.Broadcast(S.NPCId, State);
		}
	}
}

bool UCT_NPCScheduleSubsystem::ResolveForBlock(const FCT_NPCSchedule& S, ECT_TimeBlock Block, FCT_NPCScheduleResolvedState& OutState) const
{
	OutState.NPCId = S.NPCId;
	OutState.CurrentBlock = Block;

	// v0: first matching entry wins
	for (const FCT_NPCScheduleEntry& E : S.Entries)
	{
		if (E.TimeBlock == Block)
		{
			OutState.LocationTag = E.LocationTag;
			OutState.ActivityTag = E.ActivityTag;
			return true;
		}
	}

	// v0 fallback: if no entry, don't emit anything (keeps debug clean)
	return false;
}
