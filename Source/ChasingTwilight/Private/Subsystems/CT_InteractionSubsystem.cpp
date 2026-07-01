// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CT_InteractionSubsystem.h"
#include "Interaction/CT_InteractionRegistry.h"
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
UCT_InteractionSubsystem::ResolveBestInteraction(APawn* PlayerPawn) const
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