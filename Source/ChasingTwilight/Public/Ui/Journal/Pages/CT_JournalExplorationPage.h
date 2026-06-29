// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ui/Journal/CT_JournalPage.h"
#include "CT_JournalExplorationPage.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGTWILIGHT_API UCT_JournalExplorationPage : public UCT_JournalPage
{
	GENERATED_BODY()
	
public:

	virtual void Refresh() override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void RebuildDiscoveryList();
};
