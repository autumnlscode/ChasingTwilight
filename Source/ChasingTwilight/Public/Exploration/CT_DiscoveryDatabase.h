

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CT_DiscoveryDatabase.generated.h"

class UCT_DiscoveryDefinition;



UCLASS()
class CHASINGTWILIGHT_API UCT_DiscoveryDatabase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Discovery")
	TArray<TObjectPtr<UCT_DiscoveryDefinition>> Discoveries;

	UFUNCTION(BlueprintCallable)
	UCT_DiscoveryDefinition* FindDefinition(FName DiscoveryID) const;

	UFUNCTION(BlueprintPure)
	int32 GetDefinitionCount() const;

};
