#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "CT_TimeSubsystem.h" // for ECT_Season
#include "CT_SaveGame.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CHASINGTWILIGHT_API UCT_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Save format versioning (so future-you can evolve safely)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Save")
	int32 SaveVersion = 1;

	// --- Time ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Time")
	int32 DayNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Time")
	int32 TimeMinutes = 480; // 08:00 default-ish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Time")
	ECT_Season Season = ECT_Season::Springtide;

	// --- Flags ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Flags")
	FGameplayTagContainer ActiveFlags;

	// --- Cycle ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Cycle")
	FGameplayTag CycleTag;
};
