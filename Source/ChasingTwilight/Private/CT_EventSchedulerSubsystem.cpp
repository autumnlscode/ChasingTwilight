#include "CT_EventSchedulerSubsystem.h"
#include "CT_TimeSubsystem.h"
#include "CT_FlagSubsystem.h"
#include "Engine/Engine.h"

void UCT_EventSchedulerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


	UE_LOG(LogTemp, Log, TEXT("[CT] EventScheduler Initialize"));
	
	// Force TimeSubsystem to be created/initialized before we fetch it
	Collection.InitializeDependency(UCT_TimeSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_FlagSubsystem::StaticClass());
	TimeSubsystem = GetGameInstance()->GetSubsystem<UCT_TimeSubsystem>();
	FlagSubsystem = GetGameInstance()->GetSubsystem<UCT_FlagSubsystem>();



	TimeSubsystem = GetGameInstance()->GetSubsystem<UCT_TimeSubsystem>();
	if (TimeSubsystem)
	{
		CachedDay = TimeSubsystem->GetDayNumber();
		CachedBlock = TimeSubsystem->GetTimeBlock();

		TimeSubsystem->OnTimeUpdated.AddUObject(this, &UCT_EventSchedulerSubsystem::HandleTimeUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CT] EventScheduler: TimeSubsystem not found"));
	}
}

void UCT_EventSchedulerSubsystem::Deinitialize()
{
	if (TimeSubsystem)
	{
		TimeSubsystem->OnTimeUpdated.RemoveAll(this);
		TimeSubsystem = nullptr;
	}

	UE_LOG(LogTemp, Log, TEXT("[CT] EventScheduler Deinitialize"));
	Super::Deinitialize();
}

FGuid UCT_EventSchedulerSubsystem::RegisterEvent(const FCT_ScheduledEvent& Event)
{
	Events.Add(Event);

	if (Event.bFireIfAlreadyTrue && TimeSubsystem)
	{
		EvaluateAndFire(TimeSubsystem->GetDayNumber(), TimeSubsystem->GetTimeBlock());
	}

	return Event.EventId;
}

bool UCT_EventSchedulerSubsystem::UnregisterEvent(FGuid EventId)
{
	const int32 Removed = Events.RemoveAll([&](const FCT_ScheduledEvent& E)
		{
			return E.EventId == EventId;
		});
	return Removed > 0;
}

void UCT_EventSchedulerSubsystem::ClearAllEvents()
{
	Events.Reset();
	LastFiredEventName = NAME_None;
	UE_LOG(LogTemp, Log, TEXT("[CT] Scheduler: Cleared all events"));
}


static FString CT_RuleToString(ECT_ScheduleRule Rule)
{
	switch (Rule)
	{
	case ECT_ScheduleRule::OnTimeBlockEnter:     return TEXT("OnBlock");
	case ECT_ScheduleRule::DayAndTimeBlock:      return TEXT("Day+Block");
	case ECT_ScheduleRule::EveryDayAtTimeBlock:  return TEXT("EveryDay");
	default:                                     return TEXT("Unknown");
	}
}

static FString CT_BlockToString(ECT_TimeBlock Block)
{
	switch (Block)
	{
	case ECT_TimeBlock::Dawn:        return TEXT("Dawn");
	case ECT_TimeBlock::Morning:     return TEXT("Morning");
	case ECT_TimeBlock::Midday:      return TEXT("Midday");
	case ECT_TimeBlock::Afternoon:   return TEXT("Afternoon");
	case ECT_TimeBlock::Dusk:        return TEXT("Dusk");
	case ECT_TimeBlock::Evening:     return TEXT("Evening");
	case ECT_TimeBlock::Night:       return TEXT("Night");
	case ECT_TimeBlock::DarkestHour: return TEXT("DarkestHour");
	default:                         return TEXT("?");
	}
}

TArray<FCT_ScheduledEvent> UCT_EventSchedulerSubsystem::GetScheduledEvents() const
{
	return Events; // returns a copy (safe)
}

TArray<FText> UCT_EventSchedulerSubsystem::GetScheduledEventDebugLines() const
{
	TArray<FText> Out;
	Out.Reserve(Events.Num());

	for (const FCT_ScheduledEvent& E : Events)
	{
		FString Line = FString::Printf(
			TEXT("%s | %s | %s"),
			*E.DebugName.ToString(),
			*CT_RuleToString(E.Rule),
			*CT_BlockToString(E.TimeBlock)
		);

		// Only show the day when the rule uses it
		if (E.Rule == ECT_ScheduleRule::DayAndTimeBlock)
		{
			Line += FString::Printf(TEXT(" | Day=%d"), E.TargetDay);
		}

		// Only show tag if it’s actually set
		if (E.RequiredTag.IsValid())
		{
			Line += FString::Printf(TEXT(" | Req=%s"), *E.RequiredTag.ToString());
		}

		// Flags (only show when “special”)
		if (!E.bOneShot)
		{
			Line += TEXT(" | Repeat");
		}
		if (E.bFireIfAlreadyTrue)
		{
			Line += TEXT(" | FireIfTrue");
		}

		// Always show short ID suffix so you can distinguish duplicates
		Line += FString::Printf(TEXT(" | ID=%s"), *E.EventId.ToString(EGuidFormats::Short));

		Out.Add(FText::FromString(Line));
	}

	return Out;
}

FText UCT_EventSchedulerSubsystem::GetScheduledEventDebugMultiline() const
{
	if (Events.Num() == 0)
	{
		return FText::FromString(TEXT("(none)"));
	}

	const TArray<FText> Lines = GetScheduledEventDebugLines();
	FString Combined;
	for (int32 i = 0; i < Lines.Num(); ++i)
	{
		Combined += Lines[i].ToString();
		if (i < Lines.Num() - 1)
		{
			Combined += TEXT("\n");
		}
	}
	return FText::FromString(Combined);
}



void UCT_EventSchedulerSubsystem::HandleTimeUpdated(int32 NewDay, int32 NewMinutes, ECT_TimeBlock NewBlock)
{
	UE_LOG(LogTemp, Log, TEXT("[CT] Scheduler got time update: Day %d, Min %d"), NewDay, NewMinutes);
	const bool bDayChanged = (NewDay != CachedDay);
	const bool bBlockChanged = (NewBlock != CachedBlock);

	CachedDay = NewDay;

	if (bDayChanged || bBlockChanged)
	{
		CachedBlock = NewBlock;
		EvaluateAndFire(NewDay, NewBlock);
	}
}

void UCT_EventSchedulerSubsystem::EvaluateAndFire(int32 Day, ECT_TimeBlock Block)
{
	for (int32 i = Events.Num() - 1; i >= 0; --i)
	{
		const FCT_ScheduledEvent& E = Events[i];
		bool bShouldFire = false;

		// RequiredTag gate
		if (E.RequiredTag.IsValid())
		{
			const bool bHas = (FlagSubsystem && FlagSubsystem->HasFlag(E.RequiredTag));
			if (!bHas)
			{
				continue; // event is locked out
			}
		}


		switch (E.Rule)
		{
		case ECT_ScheduleRule::OnTimeBlockEnter:
			bShouldFire = (E.TimeBlock == Block);
			break;

		case ECT_ScheduleRule::DayAndTimeBlock:
			bShouldFire = (E.TargetDay == Day && E.TimeBlock == Block);
			break;

		case ECT_ScheduleRule::EveryDayAtTimeBlock:
			bShouldFire = (E.TimeBlock == Block);
			break;

		default:
			break;
		}

		if (bShouldFire)
		{
			UE_LOG(LogTemp, Log, TEXT("[CT] Scheduled Event Fired: %s"), *E.DebugName.ToString());
			LastFiredEventName = E.DebugName;
			OnEventFired.Broadcast(E);

			if (E.bOneShot)
			{
				Events.RemoveAt(i);
			}
		}
	}
}
