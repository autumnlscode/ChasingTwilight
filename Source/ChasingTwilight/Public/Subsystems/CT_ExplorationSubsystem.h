// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Exploration/CT_DiscoveryTypes.h"
#include "Exploration/CT_DiscoveryDatabase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_ExplorationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDiscoveryAdded,
	const FCTDiscoveryRecord&, Discovery
);
DECLARE_LOG_CATEGORY_EXTERN(LogCTExploration, Log, All);


UCLASS()
class CHASINGTWILIGHT_API UCT_ExplorationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "CT|Exploration")
	bool Discover(const FCTDiscoveryRequest& Request);

	UFUNCTION(BlueprintPure, Category = "CT|Exploration")
	bool HasDiscovered(FName DiscoveryID) const;

	UPROPERTY(BlueprintAssignable)
	FOnDiscoveryAdded OnDiscoveryAdded;

	UFUNCTION(BlueprintPure, Category = "CT|Exploration")
	TArray<FCTDiscoveryRecord> GetAllDiscoveries() const;

	UFUNCTION(BlueprintPure, Category= "CT|Exploration")
	FText GetDiscoveryDisplayName(FName DiscoveryID) const;

	UFUNCTION(BlueprintPure, Category = "CT|Exploration")
	bool IsValidDiscoveryID(FName DiscoveryID) const;

	UFUNCTION(BlueprintPure)
	UCT_DiscoveryDefinition* GetDiscoveryDefinition(FName DiscoveryID) const;

	UPROPERTY(EditDefaultsOnly, Category = "CT|Exploration")
	TObjectPtr<UCT_DiscoveryDatabase> DiscoveryDatabase;

private:
	UPROPERTY()
	TMap<FName, FCTDiscoveryRecord> Discoveries;

	FCTDiscoveryRecord BuildDiscoveryRecord(
		const FCTDiscoveryRequest& Request) const;

	void ValidateDiscoveryDatabase() const;
};
