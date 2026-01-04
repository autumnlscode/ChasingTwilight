#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_TimeSubsystem.generated.h"

// --- Enums kept here for simplicity ---
UENUM(BlueprintType)
enum class ECT_Season : uint8
{
    Springtide UMETA(DisplayName = "Springtide"),
    Suncrest   UMETA(DisplayName = "Suncrest"),
    Amberfall  UMETA(DisplayName = "Amberfall"),
    Frostwane  UMETA(DisplayName = "Frostwane"),
};

UENUM(BlueprintType)
enum class ECT_TimeBlock : uint8
{
    DarkestHour UMETA(DisplayName = "Darkest Hour"),
    Dawn        UMETA(DisplayName = "Dawn"),
    Morning     UMETA(DisplayName = "Morning"),
    Midday      UMETA(DisplayName = "Midday"),
    Afternoon   UMETA(DisplayName = "Afternoon"),
    Dusk        UMETA(DisplayName = "Dusk"),
    Evening     UMETA(DisplayName = "Evening"),
    Night       UMETA(DisplayName = "Night"),
    
};

// Native delegate (fast, C++ only). Great for scheduler.
DECLARE_MULTICAST_DELEGATE_ThreeParams(FCT_OnTimeUpdated, int32 /*Day*/, int32 /*Minutes*/, ECT_TimeBlock /*Block*/);


UCLASS()
class CHASINGTWILIGHT_API UCT_TimeSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    
    // Public API you already have
    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void SetTimeMinutes(int32 NewMinutes);

    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void AdvanceMinutes(int32 DeltaMinutes);

    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void SetDayNumber(int32 NewDay);

    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void SetSeason(ECT_Season NewSeason);

    // Getters (needed by overlay/scheduler)
    UFUNCTION(BlueprintPure, Category = "CT|Time")
    int32 GetTimeMinutes() const { return TimeMinutes; }

    UFUNCTION(BlueprintPure, Category = "CT|Time")
    int32 GetDayNumber() const { return DayNumber; }

    UFUNCTION(BlueprintPure, Category = "CT|Time")
    ECT_Season GetSeason() const { return Season; }

    UFUNCTION(BlueprintPure, Category = "CT|Time")
    ECT_TimeBlock GetTimeBlock() const { return CachedTimeBlock; }

    // Scheduler subscribes to this
    FCT_OnTimeUpdated OnTimeUpdated;


private:
    void NormalizeTime();

    // If you already have a different time-block function name, use that instead
    ECT_TimeBlock CalculateTimeBlock() const;

private:
    UPROPERTY()
    int32 TimeMinutes = 480; // 08:00 default

    UPROPERTY()
    int32 DayNumber = 1;

    UPROPERTY()
    ECT_Season Season = ECT_Season::Springtide;

    // Cached so we can detect transitions
    UPROPERTY()
    ECT_TimeBlock CachedTimeBlock = ECT_TimeBlock::Morning;
};
