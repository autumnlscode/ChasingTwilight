#pragma once

#include "CoreMinimal.h"
#include "Exploration/CT_DiscoveryDatabase.h"
#include "Engine/GameInstance.h"
#include "CT_GameInstance.generated.h"

UCLASS(BlueprintType)
class CHASINGTWILIGHT_API UCT_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;
    
    UCT_DiscoveryDatabase* GetDiscoveryDatabase() const
    {
        return DiscoveryDatabase;
    }
    
private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCT_DiscoveryDatabase> DiscoveryDatabase;
};
