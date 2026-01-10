#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/CT_TimeSubsystem.h" // for ECT_TimeBlock
#include "CT_SchedulerTypes.generated.h"

UENUM(BlueprintType)
enum class ECT_ScheduleRule : uint8
{
	OnTimeBlockEnter     UMETA(DisplayName = "On TimeBlock Enter"),
	DayAndTimeBlock      UMETA(DisplayName = "On Day + TimeBlock"),
	EveryDayAtTimeBlock  UMETA(DisplayName = "Every Day At TimeBlock"),
};

USTRUCT(BlueprintType)
struct FCT_ScheduledEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	FGuid EventId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	FName DebugName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	ECT_ScheduleRule Rule = ECT_ScheduleRule::OnTimeBlockEnter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	ECT_TimeBlock TimeBlock = ECT_TimeBlock::Morning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	int32 TargetDay = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	FGameplayTag RequiredTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	bool bOneShot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Schedule")
	bool bFireIfAlreadyTrue = false;

	FCT_ScheduledEvent()
	{
		EventId = FGuid::NewGuid();
	}
};
