#pragma once

#include "CoreMinimal.h"
#include "Time/CT_TimeTypes.h"
#include "Time/CT_TimeBlocks.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_TimeSubsystem.generated.h"


// Native C++
DECLARE_MULTICAST_DELEGATE_ThreeParams(
    FCT_OnTimeUpdatedNative,
    int32,
    int32,
    ECT_TimeBlocks);

// Blueprint
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FCT_OnTimeUpdated,
    int32, Day,
    int32, Minutes,
    ECT_TimeBlocks, TimeBlock);

//Region Profiles
USTRUCT(BlueprintType)
struct FCTTimeRegionSettings
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MetersPerMinute = 25.f;
};

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
    ECT_TimeBlocks GetTimeBlock() const { return CachedTimeBlock; }

    // Scheduler subscribes to this
    UPROPERTY(BlueprintAssignable, Category = "CT|Time")
    FCT_OnTimeUpdated OnTimeUpdated;

    FCT_OnTimeUpdatedNative OnTimeUpdatedNative;

    /* Eventual Broadcast events
    
    UPROPERTY(BlueprintAssignable)
    FCT_OnMinuteChanged OnMinuteChanged;

    UPROPERTY(BlueprintAssignable)
    FCT_OnTimeBlockChanged OnTimeBlockChanged;

    UPROPERTY(BlueprintAssignable)
    FCT_OnDayChanged OnDayChanged;

    UPROPERTY(BlueprintAssignable)
    FCT_OnSeasonChanged OnSeasonChanged;*/



    UFUNCTION(BlueprintPure, Category = "CT|Time")
    FCTCurrentDateTime GetCurrentDatetime() const;

    // Clock Control
    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void StartClock();

    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void StopClock();

    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void PauseClock(bool bPause);

    UFUNCTION(BlueprintPure, Category = "CT|Time")
    bool IsClockRunning() const;

    // Time Scale
    UFUNCTION(BlueprintCallable, Category = "CT|Time")
    void SetSecondsPerGameMinute(float NewSeconds);

    UFUNCTION(BlueprintPure, Category = "CT|Time")
    float GetSecondsPerGameMinute() const;

    //Ambient Time
    UPROPERTY(EditAnywhere, Category = "CT|Time")
    float MetersPerMinute = 1.f;

    float DistanceAccumulator = 0.f;
    
    UFUNCTION(BlueprintCallable)
    void AddTravelDistance(float DistanceMeters);
   
   //Intentional Time
    UFUNCTION(BlueprintCallable)
    void SpendTime(int32 Minutes);

    //Commited Time
    UFUNCTION(BlueprintCallable)
    void AdvanceToTimeBlock(ECT_TimeBlocks NewBlock);

private:
    void NormalizeTime();

    // If you already have a different time-block function name, use that instead
    ECT_TimeBlocks CalculateTimeBlock() const;

private:
    UPROPERTY()
    int32 TimeMinutes = 480; // 08:00 default

    UPROPERTY()
    int32 DayNumber = 1;

    UPROPERTY()
    ECT_Season Season = ECT_Season::Springtide;

    bool bClockRunning = true;

    bool bClockPaused = false;

    float SecondsPerGameMinute = 2.0f;

    float AccumulatedSeconds = 0.0f;

    // Cached so we can detect transitions
    UPROPERTY()
    ECT_TimeBlocks CachedTimeBlock = ECT_TimeBlocks::Morning;
};
