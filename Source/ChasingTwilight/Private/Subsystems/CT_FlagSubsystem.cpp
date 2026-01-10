#include "Subsystems/CT_FlagSubsystem.h"

void UCT_FlagSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[CT] FlagSubsystem Initialize"));
}

void UCT_FlagSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("[CT] FlagSubsystem Deinitialize"));
	Super::Deinitialize();
}

bool UCT_FlagSubsystem::HasFlag(FGameplayTag Flag) const
{
	return Flag.IsValid() && ActiveFlags.HasTagExact(Flag);
}

int32 UCT_FlagSubsystem::GetFlagCount() const
{
	return GetAllFlags().Num();
}


bool UCT_FlagSubsystem::SetFlag(FGameplayTag Flag, bool bIsSet)
{
	if (!Flag.IsValid())
	{
		return false;
	}

	const bool bWasSet = ActiveFlags.HasTagExact(Flag);
	if (bWasSet == bIsSet)
	{
		return false; // no change
	}

	if (bIsSet)
	{
		ActiveFlags.AddTag(Flag);
	}
	else
	{
		ActiveFlags.RemoveTag(Flag);
	}

	OnFlagChanged.Broadcast(Flag, bIsSet);
	UE_LOG(LogTemp, Log, TEXT("[CT] Flag %s -> %s"), *Flag.ToString(), bIsSet ? TEXT("SET") : TEXT("CLEARED"));
	return true;
}

bool UCT_FlagSubsystem::ToggleFlag(FGameplayTag Flag)
{
	return SetFlag(Flag, !HasFlag(Flag));
}

void UCT_FlagSubsystem::ClearAllFlags()
{
	ActiveFlags.Reset();
	UE_LOG(LogTemp, Log, TEXT("[CT] Flags cleared"));
}

FGameplayTagContainer UCT_FlagSubsystem::GetAllFlags() const
{
	return ActiveFlags; // copy for BP/debug
}
