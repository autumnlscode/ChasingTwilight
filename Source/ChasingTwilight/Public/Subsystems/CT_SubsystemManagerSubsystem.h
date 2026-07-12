#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Subsystems/CT_EventSchedulerSubsystem.h"
#include "Subsystems/CT_SaveSubsystem.h"
#include "Subsystems/CT_FlagSubsystem.h"
#include "Subsystems/CT_TimeSubsystem.h"
#include "Subsystems/CT_GameFlowSubsystem.h"
#include "Time/CT_TimeBlocks.h"

#include "Core/Conditions/CT_ConditionTypes.h"
#include "CT_SubsystemManagerSubsystem.generated.h"

class UCT_TimeSubsystem;
class UCT_FlagSubsystem;
class UCT_GameFlowSubsystem;
class UCT_EventSchedulerSubsystem;
class UCT_SaveSubsystem;

USTRUCT(BlueprintType)
struct FCT_WorldStateSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) int32 DayNumber = 1;
	UPROPERTY(BlueprintReadOnly) int32 TimeMinutes = 0;

	UPROPERTY(BlueprintReadOnly) ECT_Season Season = ECT_Season::Springtide;
	UPROPERTY(BlueprintReadOnly) ECT_TimeBlocks TimeBlock = ECT_TimeBlocks::Morning;
	UPROPERTY(BlueprintReadOnly) ECT_GameFlowState FlowState = ECT_GameFlowState::FrontEnd;

	UPROPERTY(BlueprintReadOnly) bool bSaveDirty = false;
	UPROPERTY(BlueprintReadOnly) FString ActiveSaveSlot;
	UPROPERTY(BlueprintReadOnly) FName LastFiredEventName = NAME_None;

	UPROPERTY(BlueprintReadOnly) int32 ScheduledEventCount = 0;
	UPROPERTY(BlueprintReadOnly) int32 FlagCount = 0;

	UPROPERTY(BlueprintReadOnly) TArray<FName> ScheduledEventNames;
};

USTRUCT(BlueprintType)
struct FCT_SubsystemStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FName Name = NAME_None;
	UPROPERTY(BlueprintReadOnly) bool bIsValid = false;
	UPROPERTY(BlueprintReadOnly) FString Note;
}; 

UCLASS(BlueprintType)
class CHASINGTWILIGHT_API UCT_SubsystemManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- Getters (Blueprint safe) ---
	UFUNCTION(BlueprintPure, Category = "CT|Core")
	UCT_TimeSubsystem* GetTimeSubsystem() const { return TimeSubsystem; }

	UFUNCTION(BlueprintPure, Category = "CT|Core")
	UCT_FlagSubsystem* GetFlagSubsystem() const { return FlagSubsystem; }

	UFUNCTION(BlueprintPure, Category = "CT|Core")
	UCT_GameFlowSubsystem* GetGameFlowSubsystem() const { return FlowSubsystem; }

	UFUNCTION(BlueprintPure, Category = "CT|Core")
	UCT_EventSchedulerSubsystem* GetEventSchedulerSubsystem() const { return SchedulerSubsystem; }

	UFUNCTION(BlueprintPure, Category = "CT|Core")
	UCT_SaveSubsystem* GetSaveSubsystem() const { return SaveSubsystem; }

	// --- Conditions ---
	UFUNCTION(BlueprintCallable, Category = "CT|Condition")
	bool CT_EvaluateCondition(const FCT_Condition& Cond, FString& OutFailReason) const;

	UFUNCTION(BlueprintCallable, Category = "CT|Condition")
	bool CT_EvaluateConditionGroup(const FCT_ConditionGroup& Group, FString& OutFailReason) const;

	// --- Readiness ---
	UFUNCTION(BlueprintPure, Category = "CT|Core")
	bool IsCoreReady() const;

	

	// --- Debug Snapshot ---
	UFUNCTION(BlueprintCallable, Category = "CT|Debug")
	FCT_WorldStateSnapshot GetWorldStateSnapshot() const;

	UFUNCTION(BlueprintCallable, Category = "CT|Debug")
	TArray<FCT_SubsystemStatus> GetSubsystemStatuses() const;

private:
	UPROPERTY() TObjectPtr<UCT_TimeSubsystem> TimeSubsystem;
	UPROPERTY() TObjectPtr<UCT_FlagSubsystem> FlagSubsystem;
	UPROPERTY() TObjectPtr<UCT_GameFlowSubsystem> FlowSubsystem;
	UPROPERTY() TObjectPtr<UCT_EventSchedulerSubsystem> SchedulerSubsystem;
	UPROPERTY() TObjectPtr<UCT_SaveSubsystem> SaveSubsystem;

	void CacheSubsystems();
};