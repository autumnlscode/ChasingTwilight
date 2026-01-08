// CT_SubsystemsBPL.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CT_SubsystemsBPL.generated.h"

class UCT_SubsystemManagerSubsystem;

UCLASS()
class CHASINGTWILIGHT_API UCT_SubsystemsBPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// One node. Works from Widgets, Actors, Controllers, etc.
	UFUNCTION(BlueprintPure, Category = "CT|Core", meta = (WorldContext = "WorldContextObject", DisplayName = "CT Get Subsystem Manager"))
	static UCT_SubsystemManagerSubsystem* CT_GetSubsystemManager(const UObject* WorldContextObject);
};
