#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "CT_NPCScheduleTypes.h"
#include "CT_NPCScheduleSubsystem.generated.h"

class UCT_TimeSubsystem;

USTRUCT(BlueprintType)
struct FCT_NPCScheduleResolvedState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NPCId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECT_TimeBlock CurrentBlock = ECT_TimeBlock::Morning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag LocationTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActivityTag;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCT_OnNPCScheduleApplied, FName, NPCId, FCT_NPCScheduleResolvedState, State);

UCLASS()
class CHASINGTWILIGHT_API UCT_NPCScheduleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "CT|NPCSchedule")
	void RegisterSchedule(const FCT_NPCSchedule& Schedule);

	UFUNCTION(BlueprintCallable, Category = "CT|NPCSchedule")
	bool UnregisterSchedule(FName NPCId);

	UFUNCTION(BlueprintCallable, Category = "CT|NPCSchedule")
	void ClearAllSchedules();

	UFUNCTION(BlueprintPure, Category = "CT|NPCSchedule|Debug")
	TArray<FCT_NPCScheduleResolvedState> GetAllResolvedStates() const;

	UPROPERTY(BlueprintAssignable, Category = "CT|NPCSchedule")
	FCT_OnNPCScheduleApplied OnScheduleApplied;

private:
	void HandleTimeUpdated(int32 NewDay, int32 NewMinutes, ECT_TimeBlock NewBlock);
	void ApplyAll(ECT_TimeBlock Block);
	bool ResolveForBlock(const FCT_NPCSchedule& S, ECT_TimeBlock Block, FCT_NPCScheduleResolvedState& OutState) const;

private:
	UPROPERTY()
	TObjectPtr<UCT_TimeSubsystem> TimeSubsystem;

	UPROPERTY()
	TArray<FCT_NPCSchedule> Schedules;

	UPROPERTY()
	TArray<FCT_NPCScheduleResolvedState> ResolvedStates;
};
