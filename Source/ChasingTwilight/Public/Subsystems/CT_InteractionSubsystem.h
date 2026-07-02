// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_InteractionSubsystem.generated.h"

class UCT_InteractionRegistry;
class UCT_InteractableComponent;
class APawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnFocusedInteractableChanged,
	UCT_InteractableComponent*,
	OldInteraction,
	UCT_InteractableComponent*,
	NewInteraction);

UCLASS()
class CHASINGTWILIGHT_API UCT_InteractionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UCT_InteractionRegistry* GetRegistry() const;

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	UCT_InteractableComponent* FindBestInteractable(APawn* PlayerPawn) const;

	UFUNCTION(BlueprintPure, Category = "CT|Interaction")
	UCT_InteractableComponent* GetFocusedInteraction() const;

	UPROPERTY(BlueprintAssignable)
	FOnFocusedInteractableChanged
		OnFocusedInteractableChanged;

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	void SetInteractionPawn(APawn* NewPawn);

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	void ClearInteractionPawn();

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	bool TryInteract();

	UFUNCTION(BlueprintPure, Category = "CT|Interaction")
	bool CanInteract() const { return bCanInteract; }

	UFUNCTION(BlueprintPure, Category = "CT|Interaction")
	FText GetCurrentInteractionPrompt() const
	{
		return CurrentInteractionPrompt;
	}

	UFUNCTION(BlueprintPure, Category = "CT|Interaction")
	int32 GetCurrentInteractionPriority() const
	{
		return CurrentInteractionPriority;
	}

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	void SetInteractionSuppressed(bool bSuppressed);

	UFUNCTION(BlueprintPure, Category = "CT|Interaction")
	bool IsInteractionSuppressed() const
	{
		return bInteractionSuppressed;
	}

private:

	UPROPERTY()
	TObjectPtr<UCT_InteractableComponent> FocusedInteraction = nullptr;

	UPROPERTY()
	TObjectPtr<UCT_InteractionRegistry> Registry;

	void UpdateFocusedInteraction();

	FTimerHandle FocusUpdateTimer;

	UPROPERTY()
	TObjectPtr<APawn> InteractionPawn = nullptr;

	UPROPERTY()
	bool bCanInteract = false;

	UPROPERTY()
	FText CurrentInteractionPrompt;

	UPROPERTY()
	int32 CurrentInteractionPriority = 0;

	UPROPERTY()
	bool bInteractionSuppressed = false;

};
