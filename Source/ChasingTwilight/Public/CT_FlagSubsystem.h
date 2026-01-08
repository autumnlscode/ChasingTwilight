#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "CT_FlagSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCT_OnFlagChanged, FGameplayTag, Flag, bool, bIsSet);


UCLASS()
class CHASINGTWILIGHT_API UCT_FlagSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    
	// --- Core API ---
	UFUNCTION(BlueprintPure, Category = "CT|Flags")
	bool HasFlag(FGameplayTag Flag) const;

	UFUNCTION(BlueprintCallable, Category = "CT|Flags")
	bool SetFlag(FGameplayTag Flag, bool bIsSet);

	UFUNCTION(BlueprintCallable, Category = "CT|Flags")
	bool ToggleFlag(FGameplayTag Flag);

	UFUNCTION(BlueprintCallable, Category = "CT|Flags")
	void ClearAllFlags();

	UFUNCTION(BlueprintPure, Category = "CT|Flags|Debug")
	FGameplayTagContainer GetAllFlags() const;

	UFUNCTION(BlueprintPure, Category = "CT|Flags|Debug")
	int32 GetFlagCount() const;


	UPROPERTY(BlueprintAssignable, Category = "CT|Flags")
	FCT_OnFlagChanged OnFlagChanged;

	


private:
	UPROPERTY()
	FGameplayTagContainer ActiveFlags;
};