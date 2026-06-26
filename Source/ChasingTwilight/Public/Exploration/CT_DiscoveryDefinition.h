// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CT_DiscoveryDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CHASINGTWILIGHT_API UCT_DiscoveryDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Discovery")
	FName DiscoveryID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Discovery")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Discovery", meta=(MultiLine=true))
	FText Description;
	
};
