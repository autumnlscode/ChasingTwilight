// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Journal/Pages/CT_JournalExplorationPage.h"

void UCT_JournalExplorationPage::Refresh()
{
	Super::Refresh();

	RebuildDiscoveryList();

}