#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Movement/CT_LocomotionMode.h"
#include "CT_MovementStateComponent.generated.h"

// Events (BlueprintAssignable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCT_OnLocomotionModeChanged, ECT_LocomotionMode, OldMode, ECT_LocomotionMode, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCT_OnMovementTagChanged, FGameplayTag, Tag);

/**
 * Hybrid movement state:
 * - Primary locomotion mode as an enum (mutually exclusive)
 * - Movement modifiers as gameplay tags (stackable)
 */
UCLASS(ClassGroup = (ChasingTwilight), meta = (BlueprintSpawnableComponent))
class CHASINGTWILIGHT_API UCT_MovementStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCT_MovementStateComponent();

	// --- Replication ---
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --- Accessors ---
	UFUNCTION(BlueprintPure, Category = "CT|MovementState")
	ECT_LocomotionMode GetLocomotionMode() const { return LocomotionMode; }

	UFUNCTION(BlueprintPure, Category = "CT|MovementState")
	const FGameplayTagContainer& GetMovementTags() const { return MovementTags; }

	UFUNCTION(BlueprintPure, Category = "CT|MovementState")
	bool HasMovementTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintPure, Category = "CT|MovementState")
	bool IsSprinting() const;

	UFUNCTION(BlueprintPure, Category = "CT|MovementState")
	bool IsMovementDisabled() const;


	// --- Mutators ---
	// If you plan multiplayer: call these on server, or add Server RPC wrappers later.
	UFUNCTION(BlueprintCallable, Category = "CT|MovementState")
	bool SetLocomotionMode(ECT_LocomotionMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "CT|MovementState")
	bool AddMovementTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "CT|MovementState")
	bool RemoveMovementTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "CT|MovementState")
	void ClearMovementTags();

	// --- Events ---
	UPROPERTY(BlueprintAssignable, Category = "CT|MovementState")
	FCT_OnLocomotionModeChanged OnLocomotionModeChanged;

	UPROPERTY(BlueprintAssignable, Category = "CT|MovementState")
	FCT_OnMovementTagChanged OnMovementTagAdded;

	UPROPERTY(BlueprintAssignable, Category = "CT|MovementState")
	FCT_OnMovementTagChanged OnMovementTagRemoved;

protected:
	virtual void BeginPlay() override;

	// --- Replicated Data ---
	UPROPERTY(ReplicatedUsing = OnRep_LocomotionMode, VisibleAnywhere, Category = "CT|MovementState")
	ECT_LocomotionMode LocomotionMode = ECT_LocomotionMode::Grounded;

	// NOTE: Replicating FGameplayTagContainer is supported; keep usage simple.
	UPROPERTY(ReplicatedUsing = OnRep_MovementTags, VisibleAnywhere, Category = "CT|MovementState")
	FGameplayTagContainer MovementTags;

	// --- Rep Notifies ---
	UFUNCTION()
	void OnRep_LocomotionMode(ECT_LocomotionMode OldMode);

	UFUNCTION()
	void OnRep_MovementTags();

private:
	// Used to diff tags in OnRep_MovementTags so we can emit Added/Removed events.
	FGameplayTagContainer LastReplicatedTags;

	// Helper used by both local changes and OnRep to broadcast tag diffs.
	void BroadcastTagDiff(const FGameplayTagContainer& OldTags, const FGameplayTagContainer& NewTags);
};
