#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Dialogue/CT_DialogueTypes.h"
#include "CT_DialogueDataAsset.generated.h"

UCLASS(BlueprintType)
class CHASINGTWILIGHT_API UCT_DialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|Dialogue")
	FName StartNodeId = TEXT("Start");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|Dialogue")
	TArray<FCT_DialogueNode> Nodes;
};
