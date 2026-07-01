// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CT_InteractionRegistry.generated.h"

class UCT_InteractableComponent;

UCLASS()
class CHASINGTWILIGHT_API UCT_InteractionRegistry : public UObject
{
	GENERATED_BODY()
	
public:

	void RegisterInteractable(UCT_InteractableComponent* Interactable);

	void UnregisterInteractable(UCT_InteractableComponent* Interactable);

	void CleanupInvalidInteractables();

	const TArray<TWeakObjectPtr<UCT_InteractableComponent>>& GetInteractables() const;

private:

	UPROPERTY()
	TArray<TWeakObjectPtr<UCT_InteractableComponent>> Interactables;
};
