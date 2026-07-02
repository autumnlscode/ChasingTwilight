// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CT_InteractionOptions.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCT_InteractionOption
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interaction")
	FName ID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interaction")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interaction")
	int32 Priority = 0;

	
};
