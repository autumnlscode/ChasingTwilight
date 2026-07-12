#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CT_ConditionTypes.generated.h"

// Forward declare instead of including TimeSubsystem here.
// We only need the enum type name to compile the struct layout.
enum class ECT_TimeBlocks : uint8;

UENUM(BlueprintType)
enum class ECT_ConditionGroupLogic : uint8
{
	All UMETA(DisplayName = "All (AND)"),
	Any UMETA(DisplayName = "Any (OR)")
};

UENUM(BlueprintType)
enum class ECT_ConditionType : uint8
{
	HasFlagTag     UMETA(DisplayName = "Has Flag Tag"),
	MissingFlagTag UMETA(DisplayName = "Missing Flag Tag"),

	TimeBlockIs    UMETA(DisplayName = "Time Block Is"),
	DayNumber      UMETA(DisplayName = "Day Number"),
	TimeMinutes    UMETA(DisplayName = "Time Minutes"),
};

UENUM(BlueprintType)
enum class ECT_CompareOp : uint8
{
	Equal          UMETA(DisplayName = "=="),
	NotEqual       UMETA(DisplayName = "!="),
	Less           UMETA(DisplayName = "<"),
	LessOrEqual    UMETA(DisplayName = "<="),
	Greater        UMETA(DisplayName = ">"),
	GreaterOrEqual UMETA(DisplayName = ">="),
};

USTRUCT(BlueprintType)
struct FCT_Condition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	ECT_ConditionType Type = ECT_ConditionType::HasFlagTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	bool bInvertResult = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	ECT_CompareOp CompareOp = ECT_CompareOp::Equal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition", meta = (Categories = "CT"))
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	ECT_TimeBlocks TimeBlockValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	FString DebugNote;
};

USTRUCT(BlueprintType)
struct FCT_ConditionGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	ECT_ConditionGroupLogic Logic = ECT_ConditionGroupLogic::All;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	TArray<FCT_Condition> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	bool bInvertResult = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Condition")
	FString DebugLabel;
};
