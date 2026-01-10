#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CT_GameInstance.generated.h"

UCLASS(BlueprintType)
class CHASINGTWILIGHT_API UCT_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;
};
