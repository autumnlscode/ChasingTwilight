#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_GameFlowTypes.h"
#include "CT_GameFlowSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FCT_OnGameFlowStateChanged,
	ECT_GameFlowState, OldState,
	ECT_GameFlowState, NewState
);

UCLASS()
class CHASINGTWILIGHT_API UCT_GameFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "CT|Flow")
	ECT_GameFlowState GetState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "CT|Flow")
	void SetState(ECT_GameFlowState NewState);

	UPROPERTY(BlueprintAssignable, Category = "CT|Flow")
	FCT_OnGameFlowStateChanged OnStateChanged;

	UFUNCTION(BlueprintCallable, Category = "CT|Flow")
	bool IsInGame() const { return CurrentState == ECT_GameFlowState::InGame; }

	UFUNCTION(BlueprintCallable, Category = "CT|Flow")
	bool IsPaused() const { return CurrentState == ECT_GameFlowState::Paused; }

	UFUNCTION(BlueprintCallable, Category = "CT|Flow")
	void TogglePause();


private:
	UPROPERTY()
	ECT_GameFlowState CurrentState = ECT_GameFlowState::Boot;
};
