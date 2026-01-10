#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/Conditions/CT_ConditionTypes.h"
#include "CT_DialogueTypes.generated.h"

class UCT_DialogueDataAsset;

UENUM(BlueprintType)
enum class ECT_DialogueSideEffectType : uint8
{
	AddFlagTag    UMETA(DisplayName = "Add Flag Tag"),
	RemoveFlagTag UMETA(DisplayName = "Remove Flag Tag"),
};

USTRUCT(BlueprintType)
struct FCT_DialogueSideEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	ECT_DialogueSideEffectType Type = ECT_DialogueSideEffectType::AddFlagTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	FGameplayTag Tag;
};

USTRUCT(BlueprintType)
struct FCT_DialogueNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	FName NodeId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	FText LineText;

	// Multiple groups = AND between groups
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	TArray<FCT_ConditionGroup> ConditionGroups;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	TArray<FCT_DialogueSideEffect> SideEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	FName NextNodeId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Dialogue")
	FName FallbackNodeId = NAME_None;
};

USTRUCT(BlueprintType)
struct FCT_DialogueSessionState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Dialogue")
	TObjectPtr<UCT_DialogueDataAsset> Asset = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Dialogue")
	TWeakObjectPtr<UObject> Speaker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Dialogue")
	FName CurrentNodeId = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Dialogue")
	bool bInDialogue = false;
};