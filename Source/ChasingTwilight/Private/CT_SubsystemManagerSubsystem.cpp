#include "CT_SubsystemManagerSubsystem.h"

#include "CT_TimeSubsystem.h"
#include "CT_FlagSubsystem.h"
#include "CT_GameFlowSubsystem.h"
#include "CT_EventSchedulerSubsystem.h"
#include "CT_SaveSubsystem.h"

void UCT_SubsystemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Dependency order hint (optional, but nice)
	Collection.InitializeDependency(UCT_TimeSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_FlagSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_GameFlowSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_EventSchedulerSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_SaveSubsystem::StaticClass());

	CacheSubsystems();
}

void UCT_SubsystemManagerSubsystem::Deinitialize()
{
	TimeSubsystem = nullptr;
	FlagSubsystem = nullptr;
	FlowSubsystem = nullptr;
	SchedulerSubsystem = nullptr;
	SaveSubsystem = nullptr;

	Super::Deinitialize();
}



void UCT_SubsystemManagerSubsystem::CacheSubsystems()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		TimeSubsystem = GI->GetSubsystem<UCT_TimeSubsystem>();
		FlagSubsystem = GI->GetSubsystem<UCT_FlagSubsystem>();
		FlowSubsystem = GI->GetSubsystem<UCT_GameFlowSubsystem>();
		SchedulerSubsystem = GI->GetSubsystem<UCT_EventSchedulerSubsystem>();
		SaveSubsystem = GI->GetSubsystem<UCT_SaveSubsystem>();
	}
}

bool UCT_SubsystemManagerSubsystem::IsCoreReady() const
{
	return TimeSubsystem && FlagSubsystem && FlowSubsystem && SchedulerSubsystem && SaveSubsystem;
}

TArray<FName> UCT_SubsystemManagerSubsystem::GetMissingCoreSubsystems() const
{
	TArray<FName> Missing;

	if (!TimeSubsystem) Missing.Add(TEXT("TimeSubsystem"));
	if (!FlagSubsystem) Missing.Add(TEXT("FlagSubsystem"));
	if (!FlowSubsystem) Missing.Add(TEXT("GameFlowSubsystem"));
	if (!SchedulerSubsystem) Missing.Add(TEXT("EventSchedulerSubsystem"));
	if (!SaveSubsystem) Missing.Add(TEXT("SaveSubsystem"));

	return Missing;
}

FCT_WorldStateSnapshot UCT_SubsystemManagerSubsystem::GetWorldStateSnapshot() const
{
	FCT_WorldStateSnapshot S;

	// Flow
	if (FlowSubsystem)
	{
		S.FlowState = FlowSubsystem->GetState();
	}

	// Time
	if (TimeSubsystem)
	{
		S.DayNumber = TimeSubsystem->GetDayNumber();
		S.TimeMinutes = TimeSubsystem->GetTimeMinutes();
		S.Season = TimeSubsystem->GetSeason();
		S.TimeBlock = TimeSubsystem->GetTimeBlock();
	}

	// Save
	if (SaveSubsystem)
	{
		S.bSaveDirty = SaveSubsystem->IsDirty();
		S.ActiveSaveSlot = SaveSubsystem->GetActiveSlotName();
	}

	// Scheduler (NEW HELPERS)
	if (SchedulerSubsystem)
	{
		S.LastFiredEventName = SchedulerSubsystem->GetLastFiredEventName();
		S.ScheduledEventCount = SchedulerSubsystem->GetScheduledEventCount();

		// Optional: list names for the overlay
		S.ScheduledEventNames = SchedulerSubsystem->GetScheduledEventNames();
	}

	// Flags (NEW HELPER)
	if (FlagSubsystem)
	{
		S.FlagCount = FlagSubsystem->GetFlagCount();
	}

	return S;
}


TArray<FCT_SubsystemStatus> UCT_SubsystemManagerSubsystem::GetSubsystemStatuses() const
{
	auto MakeStatus = [](FName Name, bool bValid, const FString& Note)
		{
			FCT_SubsystemStatus R;
			R.Name = Name;
			R.bIsValid = bValid;
			R.Note = Note;
			return R;
		};

	TArray<FCT_SubsystemStatus> Out;
	Out.Add(MakeStatus(TEXT("TimeSubsystem"), !!TimeSubsystem, TimeSubsystem ? TEXT("OK") : TEXT("Missing")));
	Out.Add(MakeStatus(TEXT("FlagSubsystem"), !!FlagSubsystem, FlagSubsystem ? TEXT("OK") : TEXT("Missing")));
	Out.Add(MakeStatus(TEXT("GameFlowSubsystem"), !!FlowSubsystem, FlowSubsystem ? TEXT("OK") : TEXT("Missing")));
	Out.Add(MakeStatus(TEXT("EventSchedulerSubsystem"), !!SchedulerSubsystem, SchedulerSubsystem ? TEXT("OK") : TEXT("Missing")));
	Out.Add(MakeStatus(TEXT("SaveSubsystem"), !!SaveSubsystem, SaveSubsystem ? TEXT("OK") : TEXT("Missing")));
	return Out;
}
