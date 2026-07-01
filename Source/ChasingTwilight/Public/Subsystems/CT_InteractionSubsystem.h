// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_InteractionSubsystem.generated.h"

class UCT_InteractionRegistry;
class UCT_InteractableComponent;

UCLASS()
class CHASINGTWILIGHT_API UCT_InteractionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UCT_InteractionRegistry* GetRegistry() const;

	UFUNCTION(BlueprintCallable, Category = "CT|Interaction")
	UCT_InteractableComponent* ResolveBestInteraction(APawn* PlayerPawn) const;

private:

	UPROPERTY()
	TObjectPtr<UCT_InteractionRegistry> Registry;

};
