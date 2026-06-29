// Fill out your copyright notice in the Description page of Project Settings.
#include "Subsystems/CT_ExplorationSubsystem.h"
#include "Core/CT_GameInstance.h"


DEFINE_LOG_CATEGORY(LogCTExploration);

void UCT_ExplorationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (UCT_GameInstance* GI =
        Cast<UCT_GameInstance>(GetGameInstance()))
    {
        DiscoveryDatabase = GI->GetDiscoveryDatabase();

        UE_LOG(LogCTExploration, Display,
            TEXT("Database: %s"),
            DiscoveryDatabase ? *DiscoveryDatabase->GetName() : TEXT("NULL"));
    }


    UE_LOG(LogCTExploration, Log, TEXT("Exploration Subsystem Initialized"));

    ValidateDiscoveryDatabase();
}

void UCT_ExplorationSubsystem::Deinitialize()
{
    UE_LOG(LogCTExploration, Log, TEXT("Exploration Subsystem Shutdown"));

    Super::Deinitialize();
}

bool UCT_ExplorationSubsystem::HasDiscovered(FName DiscoveryID) const
{
    return Discoveries.Contains(DiscoveryID);
}

bool UCT_ExplorationSubsystem::Discover(const FCTDiscoveryRequest& Request)
{

    if (Request.Definition->DiscoveryID.IsNone())
    {
        UE_LOG(LogCTExploration, Warning,
            TEXT("Discover() Called with invalid DiscoveryID."));

        return false;
    }

    if (HasDiscovered(Request.Definition->DiscoveryID))
    {
        return false;
    }

    FCTDiscoveryRecord Record = BuildDiscoveryRecord(Request);

    Discoveries.Add(Request.Definition->DiscoveryID, Record);
    
    UE_LOG(LogCTExploration,
        Warning,
        TEXT("Discover called. Map Count = %d"),
        Discoveries.Num());

    OnDiscoveryAdded.Broadcast(Record);
    LastDiscoveryID = Request.Definition->DiscoveryID;

    UE_LOG(LogCTExploration, Log, TEXT("Discovered: %s"), *Request.Definition->DiscoveryID.ToString());

    return true;
}

TArray<FCTDiscoveryRecord> UCT_ExplorationSubsystem::GetAllDiscoveries() const
{
    UE_LOG(LogCTExploration,
        Warning,
        TEXT("GetAllDiscoveries called. Map contains %d discoveries."),
        Discoveries.Num());

    TArray<FCTDiscoveryRecord> Result;

    Discoveries.GenerateValueArray(Result);

    return Result;
}

void UCT_ExplorationSubsystem::LoadDiscoveries(
    const TArray<FCTDiscoveryRecord>& Records)
{
    Discoveries.Empty();

    for (const FCTDiscoveryRecord& Record : Records)
    {
        Discoveries.Add(Record.DiscoveryID, Record);
    }
}


FCTDiscoveryRecord UCT_ExplorationSubsystem::BuildDiscoveryRecord(
    const FCTDiscoveryRequest& Request) const
{
    if (!Request.Definition)
    {
        UE_LOG(LogCTExploration, Error,
            TEXT("BuildDiscoveryRecord called with null Definition."));

        return FCTDiscoveryRecord();
    }
    
    
    FCTDiscoveryRecord Record;
    
    Record.DiscoveryID = Request.Definition->DiscoveryID;
    Record.WorldLocation = Request.WorldLocation;
    Record.State = ECTDiscoveryState::Seen;

    return Record;
}

FText UCT_ExplorationSubsystem::GetDiscoveryDisplayName(FName DiscoveryID) const
{
    if (UCT_DiscoveryDefinition* Definition =
        GetDiscoveryDefinition(DiscoveryID))

    {
        return Definition->DisplayName;
    }


    return FText::FromString(TEXT("Unknown Discovery"));
}

bool UCT_ExplorationSubsystem::IsValidDiscoveryID(FName DiscoveryID) const
{
    if (!DiscoveryID.IsValid())
    {
        return false;
    }

    return true;
}




void UCT_ExplorationSubsystem::ValidateDiscoveryDatabase() const
{
    if (!DiscoveryDatabase)
    {
        UE_LOG(LogCTExploration, Error,
            TEXT("ExplorationSubsystem: No DiscoveryDatabase assigned."));

        return;
    }

    UE_LOG(LogCTExploration, Log,
        TEXT("Validating %d discovery definitions..."),
        DiscoveryDatabase->Discoveries.Num());

    TSet<FName> SeenIDs;

    for (UCT_DiscoveryDefinition* Definition :
        DiscoveryDatabase->Discoveries)
    {
        if (!Definition)
        {
            UE_LOG(LogCTExploration, Warning,
                TEXT("Discovery Database contains a null entry."));
            continue;
        }

        if (Definition->DiscoveryID.IsNone())
        {
            UE_LOG(LogCTExploration, Warning,
                TEXT("%s has an empty DiscoveryID."),
                *Definition->GetName());
            continue;
        }

        if (SeenIDs.Contains(Definition->DiscoveryID))
        {
            UE_LOG(LogCTExploration, Error,
                TEXT("Duplicate DiscoveryID: %s"),
                *Definition->DiscoveryID.ToString());
            continue;
        }

        SeenIDs.Add(Definition->DiscoveryID);
    }

    UE_LOG(LogCTExploration, Display,
        TEXT("Discovery validation complete. %d unique discoveries."),
        SeenIDs.Num());
}

UCT_DiscoveryDefinition*
UCT_ExplorationSubsystem::GetDiscoveryDefinition(FName DiscoveryID) const
{
    if (!DiscoveryDatabase)
    {
        return nullptr;
    }

    return DiscoveryDatabase->FindDefinition(DiscoveryID);
}

int32 UCT_ExplorationSubsystem::GetDiscoveryCount() const
{
    return Discoveries.Num();
}

int32 UCT_ExplorationSubsystem::GetDefinitionCount() const
{
    return DiscoveryDatabase
        ? DiscoveryDatabase->GetDefinitionCount()
        : 0;
}

FString UCT_ExplorationSubsystem::GetDatabaseName() const
{
    return DiscoveryDatabase
        ? DiscoveryDatabase->GetName()
        : TEXT("None");
}

FText UCT_ExplorationSubsystem::GetLastDiscoveryName() const
{
    if (LastDiscoveryID.IsNone())
    {
        return FText::FromString(TEXT("None"));
    }

    return GetDiscoveryDisplayName(LastDiscoveryID);
}

void UCT_ExplorationSubsystem::ClearAllDiscoveries()
{
    Discoveries.Empty();
    LastDiscoveryID = NAME_None;
}