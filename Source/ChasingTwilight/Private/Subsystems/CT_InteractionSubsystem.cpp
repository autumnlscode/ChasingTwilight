// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CT_InteractionSubsystem.h"
#include "Interaction/CT_InteractionRegistry.h"
#include "Interaction/CT_InteractableInterface.h"
#include "Interaction/CT_InteractableComponent.h"




void UCT_InteractionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Registry = NewObject<UCT_InteractionRegistry>(this);

	UE_LOG(LogTemp, Log, TEXT("Interaction Subsystem Initialized."))
}

void UCT_InteractionSubsystem::Deinitialize()
{
	Registry = nullptr;

	Super::Deinitialize();
}

UCT_InteractionRegistry* UCT_InteractionSubsystem::GetRegistry() const
{
	return Registry;
}

UCT_InteractableComponent*
UCT_InteractionSubsystem::FindBestInteractable(APawn* PlayerPawn) const
{

    //V2 Add Radius - Distance > Interaction Radius? -> Skip
    //V3 Add Facing - Distance + View Angle
    //V4 Add Priority - Distance + Angle + Priority
    //V5 Add Trace Bonus - Distance + Angle + Priority + Trace Hit
    


    if (!PlayerPawn || !Registry)
    {
        return nullptr;
    }

    Registry->CleanupInvalidInteractables();

    UCT_InteractableComponent* Best = nullptr;
    float BestDistance = TNumericLimits<float>::Max();

    for (const TWeakObjectPtr<UCT_InteractableComponent>& Entry :
        Registry->GetInteractables())
    {
        UCT_InteractableComponent* Interactable = Entry.Get();

        if (!Interactable)
        {
            continue;
        }

        if (!Interactable->IsInteractionEnabled())
        {
            continue;
        }

        const float Distance =
            FVector::Distance(
                PlayerPawn->GetActorLocation(),
                Interactable->GetInteractionLocation());

        if (Distance > Interactable->GetInteractionRadius())
        {
            continue;
        }

        if (Distance < BestDistance)
        {
            BestDistance = Distance;
            Best = Interactable;
        }
    }

    return Best;
}

UCT_InteractableComponent*
UCT_InteractionSubsystem::GetFocusedInteraction() const
{
    return FocusedInteraction;
}


void UCT_InteractionSubsystem::SetInteractionPawn(APawn* NewPawn)
{
    InteractionPawn = NewPawn;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            FocusUpdateTimer,
            this,
            &UCT_InteractionSubsystem::UpdateFocusedInteraction,
            0.1f,
            true);
    }

    UE_LOG(LogTemp, Log,
        TEXT("Interaction Pawn Set: %s"),
        *GetNameSafe(NewPawn));
}

void UCT_InteractionSubsystem::ClearInteractionPawn()
{
    InteractionPawn = nullptr;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FocusUpdateTimer);
    }

    FocusedInteraction = nullptr;
    InteractionPawn = nullptr;

    UE_LOG(LogTemp, Log,
        TEXT("Interaction Pawn Cleared"));
}


void UCT_InteractionSubsystem::UpdateFocusedInteraction()
{

    if (bInteractionSuppressed)
    {
        return;
    }

    if (!InteractionPawn)
    {
        return;
    }

    UCT_InteractableComponent* NewFocus =
        FindBestInteractable(InteractionPawn);

  

    UCT_InteractableComponent* OldFocus = FocusedInteraction;

    FocusedInteraction = NewFocus;

    bCanInteract = false;
    CurrentInteractionPrompt = FText::GetEmpty();

    UE_LOG(LogTemp, Warning,
        TEXT("Prompt after clear: '%s'"),
        *CurrentInteractionPrompt.ToString());

    CurrentInteractionPriority = 0;

    if (FocusedInteraction)
    {
        AActor* Owner = FocusedInteraction->GetOwner();

        if (Owner &&
            Owner->Implements<UCT_InteractableInterface>())
        {
            FText FailReason;

            bCanInteract =
                ICT_InteractableInterface::Execute_CanInteract(
                    Owner,
                    InteractionPawn,
                    FailReason);

            CurrentInteractionPrompt =
                ICT_InteractableInterface::Execute_GetInteractionPrompt(
                    Owner,
                    InteractionPawn);

            CurrentInteractionPriority =
                ICT_InteractableInterface::Execute_GetInteractionPriority(
                    Owner,
                    InteractionPawn);
        }
    }
    if (NewFocus != OldFocus)
    {

        UE_LOG(LogTemp, Log,
            TEXT("Focus Changed: %s -> %s"),
            *GetNameSafe(OldFocus ? OldFocus->GetOwner() : nullptr),
            *GetNameSafe(NewFocus ? NewFocus->GetOwner() : nullptr));

        OnFocusedInteractableChanged.Broadcast(
            OldFocus,
            NewFocus);

        UE_LOG(LogTemp, Warning, TEXT("Broadcasting Focus Changed"));
    }
}

bool UCT_InteractionSubsystem::TryInteract()
{

    if (bInteractionSuppressed)
    {
        return false;
    }

    if (!FocusedInteraction)
    {
        return false;
    }

    if (!InteractionPawn)
    {
        return false;
    }

    if (!bCanInteract)
    {
        return false;
    }

    FocusedInteraction->ExecuteInteraction(InteractionPawn);

    return true;
}

void UCT_InteractionSubsystem::SetInteractionSuppressed(bool bSuppressed)
{
    if (bInteractionSuppressed == bSuppressed)
    {
        return;
    }

    bInteractionSuppressed = bSuppressed;

    if (bInteractionSuppressed)
    {
        UCT_InteractableComponent* OldFocus = FocusedInteraction;

        FocusedInteraction = nullptr;
        bCanInteract = false;
        CurrentInteractionPrompt = FText::GetEmpty();
        CurrentInteractionPriority = 0;

        OnFocusedInteractableChanged.Broadcast(
            OldFocus,
            nullptr);
    }
    else
    {
        UpdateFocusedInteraction();
    }
}