#pragma once

#include "CoreMinimal.h"
#include "CT_TimeBlockTypes.generated.h"

UENUM(BlueprintType)
enum class ECT_TimeBlockTypes : uint8
{
    DarkestHour UMETA(DisplayName = "Darkest Hour"),
    Dawn        UMETA(DisplayName = "Dawn"),
    Morning     UMETA(DisplayName = "Morning"),
    Midday      UMETA(DisplayName = "Midday"),
    Afternoon   UMETA(DisplayName = "Afternoon"),
    Evening     UMETA(DisplayName = "Evening"),
    Dusk        UMETA(DisplayName = "Dusk"),
    Night       UMETA(DisplayName = "Night"),
};