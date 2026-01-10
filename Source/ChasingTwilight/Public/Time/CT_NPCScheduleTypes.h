#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/CT_TimeSubsystem.h"
#include "CT_NPCScheduleTypes.generated.h"

USTRUCT(BlueprintType)
struct FCT_NPCScheduleEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECT_TimeBlock TimeBlock = ECT_TimeBlock::Morning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag LocationTag;     // e.g. CT.Location.Town.Tavern

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActivityTag;     // e.g. CT.Activity.Work.Bartend

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0;           // higher wins if multiple match (later)
};

USTRUCT(BlueprintType)
struct FCT_NPCSchedule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NPCId = NAME_None;      // v0: simple id (later can be GameplayTag)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCT_NPCScheduleEntry> Entries;
};
