// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/CT_InteractionRegistry.h"
#include "Interaction/CT_InteractableComponent.h"


void UCT_InteractionRegistry::RegisterInteractable(UCT_InteractableComponent* Interactable)
{
    if (!IsValid(Interactable))
    {
        return;
    }

    Interactables.AddUnique(Interactable);
    UE_LOG(LogTemp, Log,
        TEXT("Registered: %s (Total: %d)"),
        *GetNameSafe(Interactable->GetOwner()),
        Interactables.Num());
    
}

void UCT_InteractionRegistry::UnregisterInteractable(UCT_InteractableComponent* Interactable)
{
    if (!Interactable)
    {
        return;
    }

    Interactables.RemoveAll(
        [Interactable](const TWeakObjectPtr<UCT_InteractableComponent>& Entry)
        {
            return Entry.Get() == Interactable;
        });

    UE_LOG(LogTemp, Log,
        TEXT("Unregistered: %s (Total: %d)"),
        *GetNameSafe(Interactable->GetOwner()),
        Interactables.Num());
}



void UCT_InteractionRegistry::CleanupInvalidInteractables()
{
    Interactables.RemoveAll(
        [](const TWeakObjectPtr<UCT_InteractableComponent>& Entry)
        {
            return !Entry.IsValid();
        });
}

const TArray<TWeakObjectPtr<UCT_InteractableComponent>>&
UCT_InteractionRegistry::GetInteractables() const
{
    return Interactables;
}