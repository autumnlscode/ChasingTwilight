#pragma once

#include "CoreMinimal.h"
#include "CT_CameraMode.generated.h"

UENUM(BlueprintType)
enum class ECT_CameraMode : uint8
{
	Exploration UMETA(DisplayName = "Exploration"),
	Indoor      UMETA(DisplayName = "Indoor"),
	Focus       UMETA(DisplayName = "Focus"),
	Dialogue    UMETA(DisplayName = "Dialogue"),
};
