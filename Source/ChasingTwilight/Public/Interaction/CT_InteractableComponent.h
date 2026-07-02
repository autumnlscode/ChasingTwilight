// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CT_InteractionOptions.h"
#include "CT_InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnInteracted,
	APawn*,
	Interactor);


UCLASS( ClassGroup=(CT), meta=(BlueprintSpawnableComponent) )
class CHASINGTWILIGHT_API UCT_InteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCT_InteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:	
	

	UFUNCTION(BlueprintPure, Category="CT|Interaction")
	bool IsInteractionEnabled() const;

	UFUNCTION(BlueprintPure, Category="CT|Interaction")
	FVector GetInteractionLocation() const;

	UFUNCTION(BlueprintPure, Category="CT|Interaction")
	float GetInteractionRadius() const;

	UFUNCTION(BlueprintPure, Category="CT|Interaction")
	FText GetInteractionName() const;

	UFUNCTION(BlueprintPure, Category = "CT|Interaction")
	int32 GetInteractionPriority() const;

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	void SetInteractionEnabled(bool bEnabled);

	

	UPROPERTY(BlueprintAssignable)
	FOnInteracted OnInteracted;

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	TArray<FCT_InteractionOption> GetInteractionOptions(APawn* Interactor) const;

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	void ExecuteInteraction(APawn* Interactor);

	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|Interaction")
	bool bInteractionEnabled = true;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|Interaction")
	FText InteractionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|Interaction")
	int32 InteractionPriority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CT|Interaction")
	float InteractionRadius = 100.f;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName InteractionID;



};
