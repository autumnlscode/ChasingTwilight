// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "CT_DiscoveryEntryWidget.generated.h"

class UButton;
class UTextBlock;
class UCT_DiscoveryDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDiscoveryClicked,
	FName,
	DiscoveryID);

UCLASS()
class CHASINGTWILIGHT_API UCT_DiscoveryEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = "CT|Journal")
    void SetDiscovery(UCT_DiscoveryDefinition* Definition);

    UFUNCTION(BlueprintCallable, Category = "CT|Journal")
    void SetSelected(bool bNewSelected);

    UPROPERTY(BlueprintAssignable, Category = "CT|Journal")
    FOnDiscoveryClicked OnDiscoveryClicked;

protected:

    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void HandleClicked();

    UFUNCTION(BlueprintImplementableEvent, Category = "CT|Journal")
    void RefreshVisuals();

    UPROPERTY(BlueprintReadOnly, Category = "CT|Journal")
    FName DiscoveryID;

    UPROPERTY(BlueprintReadOnly, Category = "CT|Journal")
    FText DisplayName;

    UPROPERTY(BlueprintReadOnly, Category = "CT|Journal")
    FText Description;

    UPROPERTY(BlueprintReadOnly, Category = "CT|Journal")
    bool bSelected = false;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> DiscoveryButton;

};
