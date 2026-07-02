// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CT_InteractableInterface.generated.h"

class APawn;

UINTERFACE(BlueprintType)
class UCT_InteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class CHASINGTWILIGHT_API ICT_InteractableInterface
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CT|Interaction")
    bool CanInteract(APawn* Interactor, FText& FailReason) const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CT|Interaction")
    void ExecuteInteraction(APawn* Interactor);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CT|Interaction")
    FText GetInteractionPrompt(APawn* Interactor) const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CT|Interaction")
    int32 GetInteractionPriority(APawn* Interactor) const;
};