// Fill out your copyright notice in the Description page of Project Settings.

//Move Input Layers to Registry and Subsystem

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "CT_PlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCTInput, Log, All);


UENUM(BlueprintType)
enum class ECTInputMode : uint8
{
	Gameplay,
	GameplayWithCursor,
	UIOnly
};

UENUM(BlueprintType)
enum class ECTInputLayer : uint8
{
	Gameplay,
	Developer,
	Dialogue,
	Journal,
	PhotoMode,
};

USTRUCT(BlueprintType)
struct FCTInputLayer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ECTInputLayer Layer = ECTInputLayer::Gameplay;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly)
	int32 Priority = 0;
};


UCLASS()
class CHASINGTWILIGHT_API ACT_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="CT|Input")
	void ApplyInputMode(ECTInputMode NewMode);

	UFUNCTION(BlueprintCallable, Category="CT|Debug")
	void SetDeveloperCursorEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "CT|Debug")
	bool IsDeveloperCursorEnabled() const
	{
		return bDeveloperCursorEnabled;
	}
	
	UPROPERTY(EditDefaultsOnly, Category="CT|Input")
	TArray<FCTInputLayer> InputLayers;

	void AddInputLayer(ECTInputLayer Layer);
	void RemoveInputLayer(ECTInputLayer Layer);
	bool HasInputLayer(ECTInputLayer Layer) const;
	const FCTInputLayer* FindInputLayer(ECTInputLayer Layer) const;
	
	
	UPROPERTY(BlueprintReadOnly, Category = "CT|Input")
	ECTInputMode CurrentInputMode = ECTInputMode::Gameplay;
	

	UPROPERTY(EditDefaultsOnly, Category="CT|Input")
	TObjectPtr<UInputMappingContext> GameplayContext;

	UPROPERTY(EditDefaultsOnly, Category = "CT|Input")
	TObjectPtr<UInputMappingContext> DeveloperContext;

	UPROPERTY(EditDefaultsOnly, Category="CT|Input")
	int32 GameplayMappingPriority = 0;

	UFUNCTION(BlueprintCallable, Category = "CT|Debug")
	void ToggleDeveloperCursor();

	
private:

	bool bDeveloperCursorEnabled = false;

	void InitializeInputContexts();

	UPROPERTY()
	TSet<ECTInputLayer> ActiveInputLayers;
};
