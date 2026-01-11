#include "Movement/CT_MovementStateComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagsManager.h"

UCT_MovementStateComponent::UCT_MovementStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCT_MovementStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize our snapshot so OnRep diffing works cleanly.
	LastReplicatedTags = MovementTags;
}

void UCT_MovementStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCT_MovementStateComponent, LocomotionMode);
	DOREPLIFETIME(UCT_MovementStateComponent, MovementTags);
}

bool UCT_MovementStateComponent::HasMovementTag(FGameplayTag Tag) const
{
	return Tag.IsValid() && MovementTags.HasTag(Tag);
}

bool UCT_MovementStateComponent::SetLocomotionMode(ECT_LocomotionMode NewMode)
{
	if (LocomotionMode == NewMode)
	{
		return false;
	}

	const ECT_LocomotionMode OldMode = LocomotionMode;
	LocomotionMode = NewMode;

	// Local broadcast (server or standalone). Clients will also get OnRep.
	OnLocomotionModeChanged.Broadcast(OldMode, NewMode);

	return true;
}

bool UCT_MovementStateComponent::AddMovementTag(FGameplayTag Tag)
{
	if (!Tag.IsValid() || MovementTags.HasTag(Tag))
	{
		return false;
	}

	// Snapshot for diff broadcast
	const FGameplayTagContainer OldTags = MovementTags;

	MovementTags.AddTag(Tag);

	// Broadcast diff locally
	BroadcastTagDiff(OldTags, MovementTags);

	return true;
}

bool UCT_MovementStateComponent::RemoveMovementTag(FGameplayTag Tag)
{
	if (!Tag.IsValid() || !MovementTags.HasTag(Tag))
	{
		return false;
	}

	const FGameplayTagContainer OldTags = MovementTags;

	MovementTags.RemoveTag(Tag);

	BroadcastTagDiff(OldTags, MovementTags);

	return true;
}

void UCT_MovementStateComponent::ClearMovementTags()
{
	if (MovementTags.IsEmpty())
	{
		return;
	}

	const FGameplayTagContainer OldTags = MovementTags;

	MovementTags.Reset();

	BroadcastTagDiff(OldTags, MovementTags);
}

void UCT_MovementStateComponent::OnRep_LocomotionMode(ECT_LocomotionMode OldMode)
{
	// Called on clients when LocomotionMode updates
	OnLocomotionModeChanged.Broadcast(OldMode, LocomotionMode);
}

void UCT_MovementStateComponent::OnRep_MovementTags()
{
	// Diff previous replicated snapshot vs new
	BroadcastTagDiff(LastReplicatedTags, MovementTags);
	LastReplicatedTags = MovementTags;
}

void UCT_MovementStateComponent::BroadcastTagDiff(const FGameplayTagContainer& OldTags, const FGameplayTagContainer& NewTags)
{
	// Added = New - Old
	FGameplayTagContainer AddedTags = NewTags;
	AddedTags.RemoveTags(OldTags);

	// Removed = Old - New
	FGameplayTagContainer RemovedTags = OldTags;
	RemovedTags.RemoveTags(NewTags);

	// Fire events
	for (const FGameplayTag& Added : AddedTags)
	{
		OnMovementTagAdded.Broadcast(Added);
	}

	for (const FGameplayTag& Removed : RemovedTags)
	{
		OnMovementTagRemoved.Broadcast(Removed);
	}
}

bool UCT_MovementStateComponent::IsSprinting() const
{
	static const FGameplayTag SprintTag =
		FGameplayTag::RequestGameplayTag(TEXT("State.Movement.Sprinting"));

	return MovementTags.HasTag(SprintTag);
}

bool UCT_MovementStateComponent::IsMovementDisabled() const
{
	static const FGameplayTag DisabledTag =
		FGameplayTag::RequestGameplayTag(TEXT("State.Movement.Disabled"));

	return MovementTags.HasTag(DisabledTag);
}
