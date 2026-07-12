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
	UE_LOG(LogTemp, Warning,
		TEXT("AdvanceMinutes(%d)"),
		DeltaMinutes);

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

ECT_TimeBlocks UCT_TimeSubsystem::CalculateTimeBlock() const
{
	// Darkest Hour: 02:00–04:59
	if (TimeMinutes >= 120 && TimeMinutes < 300)
	{
		return ECT_TimeBlocks::DarkestHour;
	}

	// Dawn: 05:00–06:59
	if (TimeMinutes >= 300 && TimeMinutes < 420)
	{
		return ECT_TimeBlocks::Dawn;
	}

	// Morning: 07:00–09:59
	if (TimeMinutes >= 420 && TimeMinutes < 600)
	{
		return ECT_TimeBlocks::Morning;
	}

	// Midday: 10:00–13:59
	if (TimeMinutes >= 600 && TimeMinutes < 840)
	{
		return ECT_TimeBlocks::Midday;
	}

	// Afternoon: 14:00–16:59
	if (TimeMinutes >= 840 && TimeMinutes < 1020)
	{
		return ECT_TimeBlocks::Afternoon;
	}

	// Dusk: 17:00–18:59
	if (TimeMinutes >= 1020 && TimeMinutes < 1140)
	{
		return ECT_TimeBlocks::Dusk;
	}

	// Evening: 19:00–21:59
	if (TimeMinutes >= 1140 && TimeMinutes < 1320)
	{
		return ECT_TimeBlocks::Evening;
	}

	// Night: 22:00–01:59 (wraps midnight)
	return ECT_TimeBlocks::Night;
}


void UCT_TimeSubsystem::NormalizeTime()
{
	const int32 OldDay = DayNumber;
	const ECT_TimeBlocks OldBlock = CachedTimeBlock;

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

	const ECT_TimeBlocks NewBlock = CalculateTimeBlock();
	UE_LOG(LogTemp, Log, TEXT("[CT] NormalizeTime: Day=%d Min=%d Block=%d (Old=%d)"),
		DayNumber, TimeMinutes, (int32)NewBlock, (int32)OldBlock);

	CachedTimeBlock = NewBlock;

	const bool bDayChanged = (DayNumber != OldDay);
	const bool bBlockChanged = (NewBlock != OldBlock);

	CachedTimeBlock = NewBlock;

	if (bDayChanged || bBlockChanged)
	{
		OnTimeUpdatedNative.Broadcast(DayNumber, TimeMinutes, CachedTimeBlock);
		OnTimeUpdated.Broadcast(DayNumber, TimeMinutes, CachedTimeBlock);
	}
}

FCTCurrentDateTime UCT_TimeSubsystem::GetCurrentDatetime() const
{
	FCTCurrentDateTime Result;

	Result.Year = 1;
	Result.Season = Season;
	Result.DayNumber = DayNumber;
	Result.TimeBlock = CalculateTimeBlock();
	Result.Hour = TimeMinutes / 60;
	Result.Minute = TimeMinutes % 60;

	return Result;
}

void UCT_TimeSubsystem::StartClock()
{
	bClockRunning = true;
}

void UCT_TimeSubsystem::StopClock()
{
	bClockRunning = false;
}

void UCT_TimeSubsystem::PauseClock(bool bPause)
{
	bClockPaused = bPause;
}

bool UCT_TimeSubsystem::IsClockRunning() const
{
	return bClockRunning && !bClockPaused;
}

void UCT_TimeSubsystem::SetSecondsPerGameMinute(float NewSeconds)
{
	SecondsPerGameMinute = FMath::Max(NewSeconds, 0.01f);
}

float UCT_TimeSubsystem::GetSecondsPerGameMinute() const
{
	return SecondsPerGameMinute;
}

//Ambient Time
void UCT_TimeSubsystem::AddTravelDistance(float DistanceMeters)
{
	UE_LOG(LogTemp, Warning,
		TEXT("Clock Running = %d"),
		IsClockRunning());
	UE_LOG(LogTemp, Warning,
		TEXT("Distance: %.2f  Accumulator: %.2f  Threshold: %.2f"),
		DistanceMeters,
		DistanceAccumulator,
		MetersPerMinute);

	if (!IsClockRunning())
	{
		return;
	}

	if (DistanceMeters <= 0.f)
	{
		return;
	}

	DistanceAccumulator += DistanceMeters;

	UE_LOG(LogTemp, Warning,
		TEXT("Accumulator = %f"),
		DistanceAccumulator);

	while (DistanceAccumulator >= MetersPerMinute)
	{
		UE_LOG(LogTemp, Warning, TEXT("Minute Advanced!"));

		DistanceAccumulator -= MetersPerMinute;
		AdvanceMinutes(1);
	}
}

//Intentional Time
void UCT_TimeSubsystem::SpendTime(int32 Minutes)
{
	if (Minutes <= 0)
	{
		return;
	}

	AdvanceMinutes(Minutes);
}

//Commited Time
void UCT_TimeSubsystem::AdvanceToTimeBlock(ECT_TimeBlocks NewBlock)
{
	if (NewBlock == CachedTimeBlock)
	{
		return;
	}

	int32 TargetMinutes = TimeMinutes;

	switch (NewBlock)
	{
	case ECT_TimeBlocks::DarkestHour:
		TargetMinutes = 120;
		break;

	case ECT_TimeBlocks::Dawn:
		TargetMinutes = 300;
		break;

	case ECT_TimeBlocks::Morning:
		TargetMinutes = 420;
		break;

	case ECT_TimeBlocks::Midday:
		TargetMinutes = 600;
		break;

	case ECT_TimeBlocks::Afternoon:
		TargetMinutes = 840;
		break;

	case ECT_TimeBlocks::Dusk:
		TargetMinutes = 1020;
		break;

	case ECT_TimeBlocks::Evening:
		TargetMinutes = 1140;
		break;

	case ECT_TimeBlocks::Night:
		TargetMinutes = 1320;
		break;

	default:
		return;
	}

	// If we've already passed today's target,
	// advance to tomorrow.
	if (TargetMinutes <= TimeMinutes)
	{
		AdvanceMinutes((24 * 60 - TimeMinutes) + TargetMinutes);
	}
	else
	{
		AdvanceMinutes(TargetMinutes - TimeMinutes);
	}
}