#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_DialogueSubsystem.generated.h"

UCLASS()
class CHASINGTWILIGHT_API UCT_DialogueSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Placeholder for global dialogue utilities.
    UFUNCTION(BlueprintCallable, Category = "CT|Dialogue")
    void NotifyDialogueStarted();

    UFUNCTION(BlueprintCallable, Category = "CT|Dialogue")
    void NotifyDialogueEnded();
};
