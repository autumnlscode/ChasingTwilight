#pragma once

#include "CoreMinimal.h"
#include "Exploration/CT_DiscoveryDatabase.h"
#include "UI/CT_UIRegistry.h"
#include "Engine/GameInstance.h"
#include "CT_GameInstance.generated.h"

UCLASS(BlueprintType)
class CHASINGTWILIGHT_API UCT_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CT|Exploration")
    TObjectPtr<UCT_DiscoveryDatabase> DiscoveryDatabase;
    
    UFUNCTION(BlueprintPure, Category = "CT|Exploration")
    UCT_DiscoveryDatabase* GetDiscoveryDatabase() const
    {
        return DiscoveryDatabase;
    }
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CT|UI")
    TObjectPtr<UCT_UIRegistry> UIRegistry;

    UFUNCTION(BlueprintPure, Category = "CT|UI")
    UCT_UIRegistry* GetUIRegistry() const
    {
        return UIRegistry;
    }



private:
    

};
