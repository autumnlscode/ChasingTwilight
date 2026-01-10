#include "Subsystems/CT_TimeSubsystem.h"


void UCT_TimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[CT] TimeSubsystem Initialize"));

	CachedTimeBlock = CalculateTimeBlock(); // sync cache to current minutes
	UE_LOG(LogTemp, Log, TEXT("[CT] TimeSubsystem Initialize: Min=%d Block=%d"),
		TimeMinutes, (int32)CachedTimeBlock);
}

void UCT_TimeSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("[CT] TimeSubsystem Deinitialize"));
	Super::Deinitialize();
}

void UCT_TimeSubsystem::SetTimeMinutes(int32 NewMinutes)
{
    TimeMinutes = NewMinutes;
    NormalizeTime();
}

void UCT_TimeSubsystem::AdvanceMinutes(int32 DeltaMinutes)
{
    TimeMinutes += DeltaMinutes;
    NormalizeTime();
}

void UCT_TimeSubsystem::SetDayNumber(int32 NewDay)
{
	DayNumber = FMath::Max(1, NewDay);
	NormalizeTime();
}

void UCT_TimeSubsystem::SetSeason(ECT_Season NewSeason)
{
    Season = NewSeason;
}

ECT_TimeBlock UCT_TimeSubsystem::CalculateTimeBlock() const
{
	// Darkest Hour: 02:00–04:59
	if (TimeMinutes >= 120 && TimeMinutes < 300)
	{
		return ECT_TimeBlock::DarkestHour;
	}

	// Dawn: 05:00–06:59
	if (TimeMinutes >= 300 && TimeMinutes < 420)
	{
		return ECT_TimeBlock::Dawn;
	}

	// Morning: 07:00–09:59
	if (TimeMinutes >= 420 && TimeMinutes < 600)
	{
		return ECT_TimeBlock::Morning;
	}

	// Midday: 10:00–13:59
	if (TimeMinutes >= 600 && TimeMinutes < 840)
	{
		return ECT_TimeBlock::Midday;
	}

	// Afternoon: 14:00–16:59
	if (TimeMinutes >= 840 && TimeMinutes < 1020)
	{
		return ECT_TimeBlock::Afternoon;
	}

	// Dusk: 17:00–18:59
	if (TimeMinutes >= 1020 && TimeMinutes < 1140)
	{
		return ECT_TimeBlock::Dusk;
	}

	// Evening: 19:00–21:59
	if (TimeMinutes >= 1140 && TimeMinutes < 1320)
	{
		return ECT_TimeBlock::Evening;
	}

	// Night: 22:00–01:59 (wraps midnight)
	return ECT_TimeBlock::Night;
}


void UCT_TimeSubsystem::NormalizeTime()
{
	const int32 OldDay = DayNumber;
	const ECT_TimeBlock OldBlock = CachedTimeBlock;

	// Wrap across days
	while (TimeMinutes >= 24 * 60)
	{
		TimeMinutes -= 24 * 60;
		DayNumber++;
	}
	while (TimeMinutes < 0)
	{
		TimeMinutes += 24 * 60;
		DayNumber = FMath::Max(1, DayNumber - 1);
	}

	const ECT_TimeBlock NewBlock = CalculateTimeBlock();
	UE_LOG(LogTemp, Log, TEXT("[CT] NormalizeTime: Day=%d Min=%d Block=%d (Old=%d)"),
		DayNumber, TimeMinutes, (int32)NewBlock, (int32)OldBlock);

	CachedTimeBlock = NewBlock;

	const bool bDayChanged = (DayNumber != OldDay);
	const bool bBlockChanged = (NewBlock != OldBlock);

	if (bDayChanged || bBlockChanged)
	{
		OnTimeUpdated.Broadcast(DayNumber, TimeMinutes, NewBlock);
	}
}

