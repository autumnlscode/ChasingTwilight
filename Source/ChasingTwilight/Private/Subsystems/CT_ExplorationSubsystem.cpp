// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CT_ExplorationSubsystem.h"

void UCT_ExplorationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("Exploration Subsystem Initialized"));
}

void UCT_ExplorationSubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("Exploration Subsystem Shutdown"));

    Super::Deinitialize();
}

bool UCT_ExplorationSubsystem::HasDiscovered(FName DiscoveryID) const
{
    return Discoveries.Contains(DiscoveryID);
}

bool UCT_ExplorationSubsystem::Discover(const FCTDiscoveryRequest& Request)
{

    if (Request.DiscoveryID.IsNone())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Discover() Called with invalid DiscoveryID."));

        return false;
    }

    if (HasDiscovered(Request.DiscoveryID))
    {
        return false;
    }

    FCTDiscoveryRecord Record = BuildDiscoveryRecord(Request);

    Discoveries.Add(Request.DiscoveryID, Record);
    OnDiscoveryAdded.Broadcast(Record);

    UE_LOG(LogTemp, Log, TEXT("Discovered: %s"), *Request.DiscoveryID.ToString());

    return true;
}

TArray<FCTDiscoveryRecord> UCT_ExplorationSubsystem::GetAllDiscoveries() const
{
    TArray<FCTDiscoveryRecord> Result;

    Discoveries.GenerateValueArray(Result);

    return Result;
}

FCTDiscoveryRecord UCT_ExplorationSubsystem::BuildDiscoveryRecord(
    const FCTDiscoveryRequest& Request) const
{
    FCTDiscoveryRecord Record;
    
    Record.DiscoveryID = Request.DiscoveryID;
    Record.WorldLocation = Request.WorldLocation;
    Record.State = ECTDiscoveryState::Seen;

    return Record;
}