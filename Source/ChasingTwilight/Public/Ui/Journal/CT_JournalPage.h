// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CT_JournalPage.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGTWILIGHT_API UCT_JournalPage : public UUserWidget
{
	GENERATED_BODY()
	

public:
	
	UFUNCTION(BlueprintCallable, Category="CT|Journal")
	virtual void Refresh();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "CT|Journal")
	void RefreshVisuals();
};
