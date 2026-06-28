// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CT_UIRegistry.h"

const FCTWidgetDefinition* UCT_UIRegistry::FindWidget(FName WidgetID) const
{
    for (const FCTWidgetDefinition& Widget : Widgets)
    {
        if (Widget.WidgetID == WidgetID)
        {
            return &Widget;
        }
    }

    return nullptr;
}