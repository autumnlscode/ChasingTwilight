#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/CT_SchedulerTypes.h"
#include "CT_EventSchedulerSubsystem.generated.h"

class UCT_TimeSubsystem;
class UCT_FlagSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCT_OnScheduledEventFired, FCT_ScheduledEvent, EventData);

UCLASS()
class CHASINGTWILIGHT_API UCT_EventSchedulerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "CT|Schedule")
	FGuid RegisterEvent(const FCT_ScheduledEvent& Event);

	UFUNCTION(BlueprintCallable, Category = "CT|Schedule")
	bool UnregisterEvent(FGuid EventId);

	UFUNCTION(BlueprintCallable, Category = "CT|Schedule")
	void ClearAllEvents();

	UPROPERTY(BlueprintAssignable, Category = "CT|Schedule")
	FCT_OnScheduledEventFired OnEventFired;


	UFUNCTION(BlueprintPure, Category = "CT|Schedule")
	int32 GetRegisteredEventCount() const { return Events.Num(); }



	UFUNCTION(BlueprintPure, Category = "CT|Schedule|Debug")
	FName GetLastFiredEventName() const { return LastFiredEventName; }



	UFUNCTION(BlueprintPure, Category = "CT|Schedule|Debug")
	TArray<FCT_ScheduledEvent> GetScheduledEvents() const;

	UFUNCTION(BlueprintPure, Category = "CT|Schedule|Debug")
	TArray<FText> GetScheduledEventDebugLines() const;

	UFUNCTION(BlueprintPure, Category = "CT|Schedule|Debug")
	FText GetScheduledEventDebugMultiline() const;

	UFUNCTION(BlueprintPure, Category = "CT|Schedule|Debug")
	int32 GetScheduledEventCount() const;

	UFUNCTION(BlueprintPure, Category = "CT|Schedule|Debug")
	TArray<FName> GetScheduledEventNames() const;



private:
	void HandleTimeUpdated(int32 NewDay, int32 NewMinutes, ECT_TimeBlocks NewBlock);
	void EvaluateAndFire(int32 Day, ECT_TimeBlocks Block); 
	


private:
	UPROPERTY()
	TObjectPtr<UCT_TimeSubsystem> TimeSubsystem;

	UPROPERTY()
	TArray<FCT_ScheduledEvent> Events;

	UPROPERTY()
	FName LastFiredEventName = NAME_None;

	UPROPERTY()
	TObjectPtr<UCT_FlagSubsystem> FlagSubsystem;

	int32 CachedDay = 1;
	ECT_TimeBlocks CachedBlock = ECT_TimeBlocks::Morning;
};
