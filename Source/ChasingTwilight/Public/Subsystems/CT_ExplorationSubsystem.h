// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Exploration/CT_DiscoveryTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_ExplorationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDiscoveryAdded,
	const FCTDiscoveryRecord&, Discovery
);


UCLASS()
class CHASINGTWILIGHT_API UCT_ExplorationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	bool Discover(const FCTDiscoveryRequest& Request);

	UFUNCTION(BlueprintPure)
	bool HasDiscovered(FName DiscoveryID) const;

	UPROPERTY(BlueprintAssignable)
	FOnDiscoveryAdded OnDiscoveryAdded;

	UFUNCTION(BlueprintPure)
	TArray<FCTDiscoveryRecord> GetAllDiscoveries() const;


private:
	UPROPERTY()
	TMap<FName, FCTDiscoveryRecord> Discoveries;

	FCTDiscoveryRecord BuildDiscoveryRecord(
		const FCTDiscoveryRequest& Request) const;
};
