#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "Exploration/CT_DiscoveryTypes.h"
#include "Subsystems/CT_TimeSubsystem.h" // for ECT_Season
#include "CT_SaveGame.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CHASINGTWILIGHT_API UCT_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Save format versioning 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Save")
	int32 SaveVersion = 2;

	// --- Player ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Player")
	bool bHasPlayerTransform = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Player")
	FTransform PlayerTransform;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Save|Exploration")
	TArray<FCTDiscoveryRecord> Discoveries;

};
