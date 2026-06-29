// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Journal/CT_DiscoveryEntryWidget.h"
#include "Exploration/CT_DiscoveryDefinition.h"
#include "Components/Button.h"

void UCT_DiscoveryEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (DiscoveryButton)
	{
		DiscoveryButton->OnClicked.AddDynamic(
			this,
			&ThisClass::HandleClicked);
	}
}

void UCT_DiscoveryEntryWidget::HandleClicked()
{
	OnDiscoveryClicked.Broadcast(DiscoveryID);
}

void UCT_DiscoveryEntryWidget::SetSelected(bool bNewSelected)
{
	bSelected = bNewSelected;

	RefreshVisuals();
}

void UCT_DiscoveryEntryWidget::SetDiscovery(
	UCT_DiscoveryDefinition* Definition)
{
	check(Definition);

	DiscoveryID = Definition->DiscoveryID;
	DisplayName = Definition->DisplayName;
	Description = Definition->Description;

	RefreshVisuals();
}

