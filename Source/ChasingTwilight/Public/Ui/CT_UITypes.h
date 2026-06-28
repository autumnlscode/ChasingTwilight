// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CT_UITypes.generated.h"

UENUM(BlueprintType)
enum class ECTUILayer : uint8
{
	World,
	HUD,
	Notification,
	Overlay,
	Menu,
	Modal,
	Developer,
};

USTRUCT(BlueprintType)
struct FCTWidgetDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName WidgetID;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere)
	ECTUILayer Layer = ECTUILayer::HUD;
};