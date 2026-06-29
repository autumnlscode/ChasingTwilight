// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CT_UISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UI/CT_UITypes.h"
#include "Core/CT_GameInstance.h"

DEFINE_LOG_CATEGORY(LogCTUI);

namespace
{
	int32 LayerToZOrder(ECTUILayer Layer)
	{
		switch (Layer)
		{
		case ECTUILayer::World:         return 0;
		case ECTUILayer::HUD:           return 100;
		case ECTUILayer::Notification:  return 200;
		case ECTUILayer::Overlay:       return 300;
		case ECTUILayer::Menu:          return 400;
		case ECTUILayer::Modal:         return 500;
		case ECTUILayer::Developer:     return 1000;
		default:                        return 0;
		}
	}
}

void UCT_UISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UCT_GameInstance* CTGameInstance =
		Cast<UCT_GameInstance>(GetGameInstance());

	if (CTGameInstance)
	{
		UIRegistry = CTGameInstance->GetUIRegistry();
		
		UE_LOG(LogCTUI,
			Display,
			TEXT("UI Registry: %s"),
			UIRegistry
			? *UIRegistry->GetName()
			: TEXT("NULL"));
	}

		
	UE_LOG(LogCTUI, Log, TEXT("UI Subsystem Initialized"));

	ValidateRegistry();

}

void UCT_UISubsystem::Deinitialize()
{
	UE_LOG(LogCTUI, Log, TEXT("UI Subsystem Shutdown"));

	Super::Deinitialize();
}


const FCTWidgetDefinition* UCT_UISubsystem::FindWidgetDefinition(FName WidgetID) const
{
	if (!UIRegistry)
	{
		return nullptr;
	}

	return UIRegistry->FindWidget(WidgetID);
}

UUserWidget* UCT_UISubsystem::GetWidget(FName WidgetID) const
{
	if (const TObjectPtr<UUserWidget>* Widget = ActiveWidget.Find(WidgetID))
	{
		return Widget->Get();
	}

	return nullptr;
}

bool UCT_UISubsystem::IsWidgetVisible(FName WidgetID) const
{
	if (UUserWidget* Widget = GetWidget(WidgetID))
	{
		return Widget->GetVisibility() == ESlateVisibility::Visible;
	}

	return false;
}

void UCT_UISubsystem::HideWidget(FName WidgetID)
{
	if (UUserWidget* Widget = GetWidget(WidgetID))
	{
		Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	UE_LOG(LogCTUI, Display,
		TEXT("Hiding Widget: %s"),
		*WidgetID.ToString());

	RefreshInputMode();

}

UUserWidget* UCT_UISubsystem::EnsureWidget(FName WidgetID)
{
	// Already exists?
	if (UUserWidget* Existing = GetWidget(WidgetID))
	{
		return Existing;
	}

	const FCTWidgetDefinition* Definition =
		FindWidgetDefinition(WidgetID);

	if (!Definition)
	{
		UE_LOG(LogCTUI, Error,
			TEXT("Unknown WidgetID: %s"),
			*WidgetID.ToString());

		return nullptr;
	}

	UUserWidget* Widget =
		::CreateWidget<UUserWidget>(
			GetWorld(),
			Definition->WidgetClass);

	if (!Widget)
	{
		return nullptr;
	}

	Widget->AddToViewport(
		LayerToZOrder(Definition->Layer));

	ActiveWidget.Add(
		WidgetID,
		Widget);

	return Widget;
}

UUserWidget* UCT_UISubsystem::ShowWidget(FName WidgetID)
{
	UUserWidget* Widget = EnsureWidget(WidgetID);

	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}

	RefreshInputMode();

	return Widget;
}

void UCT_UISubsystem::ValidateRegistry() const
{
	if (!UIRegistry)
	{
		UE_LOG(LogCTUI, Error,
			TEXT("No UI Registry assigned."));
		return;
	}

	UE_LOG(LogCTUI,
		Display,
		TEXT("Validating %d UI Widgets..."),
		UIRegistry->Widgets.Num());

	TSet<FName> IDs;

	for (const FCTWidgetDefinition& Widget :
		UIRegistry->Widgets)
	{
		if (Widget.WidgetID.IsNone())
		{
			UE_LOG(LogCTUI, Warning,
				TEXT("Widget with Empty ID."));
			continue;
		}

		if (IDs.Contains(Widget.WidgetID))
		{
			UE_LOG(LogCTUI, Error,
				TEXT("Duplicate WidgetID: %s"),
				*Widget.WidgetID.ToString());
			continue;
		}

		IDs.Add(Widget.WidgetID);

		if (!Widget.WidgetClass)
		{
			UE_LOG(LogCTUI, Error,
				TEXT("&s has no WidgetClass assigned."),
				*Widget.WidgetID.ToString());
		}
	}
}

void UCT_UISubsystem::ToggleWidget(FName WidgetID)
{
	if (IsWidgetVisible(WidgetID))
	{
		HideWidget(WidgetID);
	}
	else
	{
		ShowWidget(WidgetID);
	}
}

void UCT_UISubsystem::RefreshInputMode()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	bool bWantsUIFocus = false;
	UUserWidget* FocusWidget = nullptr;

	for (const auto& Pair : ActiveWidget)
	{
		UUserWidget* Widget = Pair.Value.Get();

		if (!Widget)
		{
			continue;
		}

		if (Widget->GetVisibility() != ESlateVisibility::Visible)
		{
			continue;
		}

		const FCTWidgetDefinition* Definition =
			FindWidgetDefinition(Pair.Key);

		if (!Definition)
		{
			continue;
		}

		if (Definition->bRequestsUIFocus)
		{
			bWantsUIFocus = true;
			FocusWidget = Widget;
			break;
		}
	}

	if (bWantsUIFocus)
	{
		PC->bShowMouseCursor = true;

		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		if (FocusWidget)
		{
			Mode.SetWidgetToFocus(FocusWidget->TakeWidget());
		}

		PC->SetInputMode(Mode);
		PC->SetIgnoreLookInput(true);
	}
	else
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly Mode;
		PC->SetInputMode(Mode);
		PC->SetIgnoreLookInput(false);
	}
}