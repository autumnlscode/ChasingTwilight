// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CT_UITypes.h"
#include "UI/CT_UIRegistry.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CT_UISubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCTUI, Log, All);







UCLASS()
class CHASINGTWILIGHT_API UCT_UISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


	UPROPERTY()
	TObjectPtr<UCT_UIRegistry> UIRegistry;

	UFUNCTION(BlueprintPure)
	UCT_UIRegistry* GetUIRegistry() const
	{
		return UIRegistry;
	}

	UPROPERTY()
	TMap<FName, TObjectPtr<UUserWidget>> ActiveWidget;

	UFUNCTION(BlueprintCallable, Category = "CT|UI")
	UUserWidget* EnsureWidget(FName WidgetID);

	UFUNCTION(BlueprintCallable, Category = "CT|UI")
	UUserWidget* ShowWidget(FName WidgetID);

	UFUNCTION(BlueprintCallable, Category = "CT|UI")
	void HideWidget(FName WidgetID);

	UFUNCTION(BlueprintPure, Category = "CT|UI")
	bool IsWidgetVisible(FName WidgetID) const;

	UFUNCTION(BlueprintPure, Category = "CT|UI")
	UUserWidget* GetWidget(FName WidgetID) const;

	UFUNCTION(BlueprintCallable)
	void ToggleWidget(FName WidgetID);

private:

	const FCTWidgetDefinition* FindWidgetDefinition(FName WidgetID) const;

	void ValidateRegistry() const;
};
