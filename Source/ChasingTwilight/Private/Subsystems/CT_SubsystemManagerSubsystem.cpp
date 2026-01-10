#include "Subsystems/CT_SubsystemManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Subsystems/CT_TimeSubsystem.h"
#include "Subsystems/CT_FlagSubsystem.h"
#include "Subsystems/CT_GameFlowSubsystem.h"
#include "Subsystems/CT_EventSchedulerSubsystem.h"
#include "Subsystems/CT_SaveSubsystem.h"

static bool CT_CompareInt(int32 A, int32 B, ECT_CompareOp Op)
{
	switch (Op)
	{
	case ECT_CompareOp::Equal:          return A == B;
	case ECT_CompareOp::NotEqual:       return A != B;
	case ECT_CompareOp::Less:           return A < B;
	case ECT_CompareOp::LessOrEqual:    return A <= B;
	case ECT_CompareOp::Greater:        return A > B;
	case ECT_CompareOp::GreaterOrEqual: return A >= B;
	default:                            return false;
	}
}

void UCT_SubsystemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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
	TimeSubsystem = GetGameInstance()->GetSubsystem<UCT_TimeSubsystem>();
	FlagSubsystem = GetGameInstance()->GetSubsystem<UCT_FlagSubsystem>();
	FlowSubsystem = GetGameInstance()->GetSubsystem<UCT_GameFlowSubsystem>();
	SchedulerSubsystem = GetGameInstance()->GetSubsystem<UCT_EventSchedulerSubsystem>();
	SaveSubsystem = GetGameInstance()->GetSubsystem<UCT_SaveSubsystem>();
}

bool UCT_SubsystemManagerSubsystem::IsCoreReady() const
{
	return IsValid(TimeSubsystem)
		&& IsValid(FlagSubsystem)
		&& IsValid(FlowSubsystem)
		&& IsValid(SchedulerSubsystem)
		&& IsValid(SaveSubsystem);
}


FCT_WorldStateSnapshot UCT_SubsystemManagerSubsystem::GetWorldStateSnapshot() const
{
	FCT_WorldStateSnapshot Snap;

	if (IsValid(TimeSubsystem))
	{
		Snap.DayNumber = TimeSubsystem->GetDayNumber();
		Snap.TimeMinutes = TimeSubsystem->GetTimeMinutes();
		Snap.TimeBlock = TimeSubsystem->GetTimeBlock();
		Snap.Season = TimeSubsystem->GetSeason();
	}

	if (IsValid(FlowSubsystem))
	{
		Snap.FlowState = FlowSubsystem->GetFlowState();
	}

	if (IsValid(SaveSubsystem))
	{
		Snap.bSaveDirty = SaveSubsystem->IsDirty();
		Snap.ActiveSaveSlot = SaveSubsystem->GetActiveSlotName(); // if yours returns FString already, great.
	}

	if (IsValid(SchedulerSubsystem))
	{
		Snap.LastFiredEventName = SchedulerSubsystem->GetLastFiredEventName();
		Snap.ScheduledEventCount = SchedulerSubsystem->GetScheduledEventCount();
		Snap.ScheduledEventNames = SchedulerSubsystem->GetScheduledEventNames();
	}

	if (IsValid(FlagSubsystem))
	{
		Snap.FlagCount = FlagSubsystem->GetAllFlags().Num();
	}

	return Snap;
}


bool UCT_SubsystemManagerSubsystem::CT_EvaluateCondition(const FCT_Condition& Cond, FString& OutFailReason) const
{
	bool bResult = false;
	OutFailReason.Empty();

	switch (Cond.Type)
	{
	case ECT_ConditionType::HasFlagTag:
		bResult = FlagSubsystem && Cond.Tag.IsValid() && FlagSubsystem->HasFlag(Cond.Tag);
		if (!bResult) OutFailReason = TEXT("Does not have flag");
		break;

	case ECT_ConditionType::MissingFlagTag:
		bResult = !FlagSubsystem || !Cond.Tag.IsValid() || !FlagSubsystem->HasFlag(Cond.Tag);
		if (!bResult) OutFailReason = TEXT("Has flag (expected missing)");
		break;

	case ECT_ConditionType::TimeBlockIs:
		if (!TimeSubsystem) { OutFailReason = TEXT("TimeSubsystem invalid"); return Cond.bInvertResult ? true : false; }
		bResult = CT_CompareInt((int32)TimeSubsystem->GetTimeBlock(), (int32)Cond.TimeBlockValue, Cond.CompareOp);
		if (!bResult) OutFailReason = TEXT("TimeBlock compare failed");
		break;

	case ECT_ConditionType::DayNumber:
		if (!TimeSubsystem) { OutFailReason = TEXT("TimeSubsystem invalid"); return Cond.bInvertResult ? true : false; }
		bResult = CT_CompareInt(TimeSubsystem->GetDayNumber(), Cond.IntValue, Cond.CompareOp);
		if (!bResult) OutFailReason = TEXT("DayNumber compare failed");
		break;

	case ECT_ConditionType::TimeMinutes:
		if (!TimeSubsystem) { OutFailReason = TEXT("TimeSubsystem invalid"); return Cond.bInvertResult ? true : false; }
		bResult = CT_CompareInt(TimeSubsystem->GetTimeMinutes(), Cond.IntValue, Cond.CompareOp);
		if (!bResult) OutFailReason = TEXT("TimeMinutes compare failed");
		break;

	default:
		OutFailReason = TEXT("Unknown condition type");
		bResult = false;
		break;
	}

	if (Cond.bInvertResult)
	{
		bResult = !bResult;
		if (!bResult && OutFailReason.IsEmpty())
		{
			OutFailReason = TEXT("Inverted result failed");
		}
	}
	return bResult;
}

TArray<FCT_SubsystemStatus> UCT_SubsystemManagerSubsystem::GetSubsystemStatuses() const
{
	TArray<FCT_SubsystemStatus> Out;

	auto AddStatus = [&Out](FName Name, UObject* Obj)
		{
			FCT_SubsystemStatus S;
			S.Name = Name;
			S.bIsValid = IsValid(Obj);
			S.Note = S.bIsValid ? TEXT("OK") : TEXT("Missing / invalid");
			Out.Add(S);
		};

	AddStatus(TEXT("TimeSubsystem"), TimeSubsystem.Get());
	AddStatus(TEXT("FlagSubsystem"), FlagSubsystem.Get());
	AddStatus(TEXT("GameFlowSubsystem"), FlowSubsystem.Get());
	AddStatus(TEXT("EventSchedulerSubsystem"), SchedulerSubsystem.Get());
	AddStatus(TEXT("SaveSubsystem"), SaveSubsystem.Get());

	return Out;
}

bool UCT_SubsystemManagerSubsystem::CT_EvaluateConditionGroup(const FCT_ConditionGroup& Group, FString& OutFailReason) const
{
	OutFailReason.Empty();

	if (Group.Conditions.Num() == 0)
	{
		const bool bPass = true;
		return Group.bInvertResult ? !bPass : bPass;
	}

	const bool bAll = (Group.Logic == ECT_ConditionGroupLogic::All);
	bool bFinal = bAll ? true : false;
	FString FirstFail;

	for (const FCT_Condition& Cond : Group.Conditions)
	{
		FString Fail;
		const bool bPass = CT_EvaluateCondition(Cond, Fail);

		if (bAll)
		{
			if (!bPass)
			{
				bFinal = false;
				FirstFail = Fail;
				break;
			}
		}
		else
		{
			if (bPass)
			{
				bFinal = true;
				break;
			}
			if (FirstFail.IsEmpty()) FirstFail = Fail;
		}
	}

	if (!bFinal && OutFailReason.IsEmpty())
	{
		OutFailReason = FirstFail.IsEmpty() ? TEXT("Condition group failed") : FirstFail;
	}

	return Group.bInvertResult ? !bFinal : bFinal;
}
