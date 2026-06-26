// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "CT_DiscoveryDefinition.h"
#include "Subsystems/CT_TimeSubsystem.h"
#include "CT_DiscoveryTypes.generated.h"


UENUM(BlueprintType)
enum class ECTDiscoveryState : uint8
{
    Seen UMETA(DisplayName = "Seen"),
    Investigated UMETA(DisplayName = "Investigated"),
    Understood UMETA(DisplayName = "Understood")
};

USTRUCT(BlueprintType)
struct FCTDiscoveryRecord
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly)
    FName DiscoveryID;

    UPROPERTY(BlueprintReadOnly)
    ECTDiscoveryState State = ECTDiscoveryState::Seen;

    UPROPERTY(BlueprintReadOnly)
    int32 DayDiscovered = 0;

    UPROPERTY(BlueprintReadOnly)
    ECT_TimeBlock TimeBlock;

    UPROPERTY(BlueprintReadOnly)
    FVector WorldLocation = FVector::ZeroVector;
};


UENUM(BlueprintType)
enum class ECTDiscoveryMethod : uint8
{
    Interaction,
    Automatic,
    Sketch,
    Dialogue,
    Ability,
    Script,
};

USTRUCT(BlueprintType)
struct FCTDiscoveryRequest
{
    GENERATED_BODY()

public:

    UPROPERTY()
    TObjectPtr<UCT_DiscoveryDefinition> Definition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WorldLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECTDiscoveryMethod Method = ECTDiscoveryMethod::Interaction;

};

