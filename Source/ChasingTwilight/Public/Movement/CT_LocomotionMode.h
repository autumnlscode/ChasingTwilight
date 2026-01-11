#pragma once

#include "CoreMinimal.h"
#include "CT_LocomotionMode.generated.h"

UENUM(BlueprintType)
enum class ECT_LocomotionMode : uint8
{
	Grounded	UMETA(DisplayName = "Grounded"),
	InAir		UMETA(DisplayName = "In Air"),
	Swimming	UMETA(DisplayName = "Swimming"),
	Underwater	UMETA(DisplayName = "Underwater"),
};
