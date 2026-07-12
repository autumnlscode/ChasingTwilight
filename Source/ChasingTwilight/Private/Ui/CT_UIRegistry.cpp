// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CT_UIRegistry.h"
#include "Subsystems/CT_UISubsystem.h"

const FCTWidgetDefinition* UCT_UIRegistry::FindWidget(FName WidgetID) const
{
    UE_LOG(LogCTUI, Warning,
        TEXT("Looking for '%s'"),
        *WidgetID.ToString());

    for (const FCTWidgetDefinition& Widget : Widgets)
    {
        UE_LOG(LogCTUI, Warning,
            TEXT("Comparing against '%s'"),
            *Widget.WidgetID.ToString());

        if (Widget.WidgetID == WidgetID)
        {
            UE_LOG(LogCTUI, Warning, TEXT("MATCH"));
            return &Widget;
        }
    }
    return nullptr;
}