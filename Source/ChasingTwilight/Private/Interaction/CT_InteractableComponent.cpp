// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/CT_InteractableComponent.h"
#include "Subsystems/CT_InteractionSubsystem.h"
#include "Interaction/CT_InteractableInterface.h"
#include "Interaction/CT_InteractionRegistry.h"
#include "Engine/GameInstance.h"

UCT_InteractableComponent::UCT_InteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UCT_InteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UCT_InteractionSubsystem* InteractionSubsystem =
			GI->GetSubsystem<UCT_InteractionSubsystem>())
		{
			InteractionSubsystem
				->GetRegistry()
				->RegisterInteractable(this);
		}
	}
	
}

void UCT_InteractableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UCT_InteractionSubsystem* InteractionSubsystem =
			GI->GetSubsystem<UCT_InteractionSubsystem>())
		{
			InteractionSubsystem
				->GetRegistry()
				->UnregisterInteractable(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}


FVector UCT_InteractableComponent::GetInteractionLocation() const
{
	FVector Origin;
	FVector Extent;

	GetOwner()->GetActorBounds(true, Origin, Extent);

	return Origin;
}

bool UCT_InteractableComponent::IsInteractionEnabled() const
{
	return bInteractionEnabled;
}

float UCT_InteractableComponent::GetInteractionRadius() const
{
	return InteractionRadius;
}

FText UCT_InteractableComponent::GetInteractionName() const
{
	return InteractionName;
}

int32 UCT_InteractableComponent::GetInteractionPriority() const
{
	return InteractionPriority;
}

void UCT_InteractableComponent::SetInteractionEnabled(bool bEnabled)
{
	bInteractionEnabled = bEnabled;
}



TArray<FCT_InteractionOption> UCT_InteractableComponent::GetInteractionOptions(APawn* Interactor) const
{
	return {};
}

void UCT_InteractableComponent::ExecuteInteraction(APawn* Interactor)
{
	AActor* OwnerActor = GetOwner();

	if (!OwnerActor)
	{
		return;
	}

	ICT_InteractableInterface::Execute_ExecuteInteraction(
		OwnerActor,
		Interactor);
}

