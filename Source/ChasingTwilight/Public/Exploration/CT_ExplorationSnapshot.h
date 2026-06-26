// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CT_DiscoveryTypes.h"
#include "CT_ExplorationSnapshot.generated.h"


USTRUCT(BlueprintType)
struct FCTExplorationSnapshot
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TArray<FCTDiscoveryRecord> Discoveries;
};
