#pragma once

#include "CoreMinimal.h"
#include "CT_GameFlowTypes.generated.h"

UENUM(BlueprintType)
enum class ECT_GameFlowState : uint8
{
	Boot     UMETA(DisplayName = "Boot"),
	FrontEnd UMETA(DisplayName = "Front End"),
	InGame   UMETA(DisplayName = "In Game"),
	Paused   UMETA(DisplayName = "Paused"),
};
