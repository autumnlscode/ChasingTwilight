#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dialogue/CT_DialogueTypes.h"
#include "Core/Conditions/CT_ConditionTypes.h"
#include "CT_DialogueSubsystem.generated.h"

class UCT_DialogueDataAsset;
class UCT_TimeSubsystem;
class UCT_FlagSubsystem;
class UCT_SubsystemManagerSubsystem;

UENUM(BlueprintType)
enum class ECT_DialogueState : uint8
{
	Started,
	Ended
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCT_OnDialogueEvent, AActor*, Speaker,	AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCT_OnDialogueStateChanged, ECT_DialogueState, State, AActor*, Speaker, AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCT_OnDialogueLineSpoken, FText, Line);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCT_OnDialogueEnded);

UCLASS()
class CHASINGTWILIGHT_API UCT_DialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="CT|Dialogue")
	bool StartDialogue(UCT_DialogueDataAsset* DialogueAsset, AActor* Speaker, AActor* Interactor);

	UFUNCTION(BlueprintCallable, Category="CT|Dialogue")
	void Advance();

	UFUNCTION(BlueprintCallable, Category="CT|Dialogue")
	void EndDialogue();

	UFUNCTION(BlueprintCallable, Category = "CT|Dialogue")
	void CloseDialogue();

	UFUNCTION(BlueprintPure, Category="CT|Dialogue")
	bool IsInDialogue() const { return Session.bInDialogue; }

	UFUNCTION(BlueprintPure) FText GetLastSpokenLine() const;

	UPROPERTY(BlueprintAssignable, Category="CT|Dialogue")
	FCT_OnDialogueLineSpoken OnLineSpoken;

	UPROPERTY(BlueprintAssignable, Category = "CT|Dialogue")
	FCT_OnDialogueEvent OnDialogueStarted;

	
	UPROPERTY(BlueprintAssignable, Category = "CT|Dialogue")
	FCT_OnDialogueEvent OnDialogueEnded;

	
	UPROPERTY(BlueprintAssignable, Category = "CT|Dialogue")
	FCT_OnDialogueStateChanged OnDialogueStateChanged;


	UFUNCTION(BlueprintPure, Category = "CT|Dialogue")
	bool FindNodeIndexById(const UCT_DialogueDataAsset* Asset, FName NodeId, int32& OutIndex) const;

	UFUNCTION(BlueprintCallable, Category = "CT|Dialogue")
	bool FindNodeById(const UCT_DialogueDataAsset* Asset, FName NodeId, FCT_DialogueNode& OutNode) const;


	UFUNCTION(BlueprintPure, Category = "CT|Dialogue")
	bool GetNodeById(const UCT_DialogueDataAsset* Asset, FName NodeId, FCT_DialogueNode& OutNode) const;


private:
	
	bool EvaluateNode(const FCT_DialogueNode& Node, FString& OutFailReason) const;
	bool EvaluateConditionGroup(const FCT_ConditionGroup& Group, FString& OutFailReason) const;
	bool EvaluateCondition(const FCT_Condition& Cond, FString& OutFailReason) const;

	void ApplyNodeSideEffects(const FCT_DialogueNode& Node);
	void SpeakNodeAndAdvanceState(const FCT_DialogueNode& Node);
	void BroadcastDialogueState(ECT_DialogueState State);

private:
	UPROPERTY()
	FCT_DialogueSessionState Session;

	UPROPERTY()
	FText LastSpokenLine;

	UPROPERTY()
	TObjectPtr<UCT_TimeSubsystem> TimeSubsystem;

	UPROPERTY()
	TObjectPtr<UCT_FlagSubsystem> FlagSubsystem;

	UPROPERTY()
	TObjectPtr<UCT_SubsystemManagerSubsystem> SubsystemManager;
	
};