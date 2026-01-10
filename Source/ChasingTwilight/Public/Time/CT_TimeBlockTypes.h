#pragma once

#include "CoreMinimal.h"
#include "CT_TimeBlockTypes.generated.h"

UENUM(BlueprintType)
enum class ECT_TimeBlockTypes : uint8
{
    None = 0	    UMETA(DisplayName = "None"),
    DarkestHour = 1 UMETA(DisplayName = "Darkest Hour"),
    Dawn = 2        UMETA(DisplayName = "Dawn"),
    Morning = 3     UMETA(DisplayName = "Morning"),
    Midday = 4      UMETA(DisplayName = "Midday"),
    Afternoon = 5   UMETA(DisplayName = "Afternoon"),
    Dusk = 6        UMETA(DisplayName = "Dusk"),
    Evening = 7     UMETA(DisplayName = "Evening"),
    Night = 8       UMETA(DisplayName = "Night"),

};