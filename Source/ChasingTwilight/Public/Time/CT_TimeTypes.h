// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CT_TimeBlocks.h"
#include "CT_TimeTypes.generated.h"

UENUM(BlueprintType)
enum class ECT_Season : uint8
{
	None = 0       UMETA(DisplayName = "None"),
	Springtide = 1 UMETA(DisplayName = "Springtide"),
	Suncrest = 2   UMETA(DisplayName = "Suncrest"),
	Amberfall = 3  UMETA(DisplayName = "Amberfall"),
	Frostwane = 4  UMETA(DisplayName = "Frostwane"),
};

enum class ECT_DayOFWeek : uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Monday = 1	UMETA(DisplayName = "Monday"),
	Tuesday = 2 UMETA(DisplayName = "Tuesday"),
	Wednesday = 3 UMETA(DisplayName = "Wednesday"),
	Thursday = 4 UMETA(DisplayName = "Thursday"),
	Friday = 5 UMETA(DisplayName = "Friday"),
	Saturday = 6 UMETA(DisplayName = "Saturday"),
	Sunday = 7 UMETA(DisplayName = "Sunday"),

};



USTRUCT(BlueprintType)
struct FCTCurrentDateTime
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category = "CT|Time")
	int32 Year = 1;

	UPROPERTY(BlueprintReadOnly, Category = "CT|Time")
	ECT_Season Season;


	UPROPERTY(BlueprintReadOnly)
	int32 DayNumber = 1;


	UPROPERTY(BlueprintReadOnly, Category = "CT|Time")
	ECT_TimeBlocks TimeBlock = ECT_TimeBlocks::Morning;
	
	UPROPERTY(BlueprintReadOnly, Category = "CT|Time")
	int32 Hour = 8;

	UPROPERTY(BlueprintReadOnly, Category = "CT|Time")
	int32 Minute = 0;


};
