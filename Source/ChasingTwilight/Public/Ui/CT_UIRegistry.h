// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "CT_UITypes.h"
#include "CT_UIRegistry.generated.h"


UCLASS()
class CHASINGTWILIGHT_API UCT_UIRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|UI")
	TArray<FCTWidgetDefinition> Widgets;

	const FCTWidgetDefinition* FindWidget(FName WidgetID) const;
};
