// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CT_PlayerController.h"
#include "Subsystems/CT_InteractionSubsystem.h"
#include "Interaction/CT_InteractableComponent.h"


DEFINE_LOG_CATEGORY(LogCTInput);




void ACT_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeInputContexts();

	AddInputLayer(ECTInputLayer::Gameplay);
	AddInputLayer(ECTInputLayer::Developer);

	if (UCT_InteractionSubsystem* InteractionSubsystem =
		GetGameInstance()->GetSubsystem<UCT_InteractionSubsystem>())
	{
		InteractionSubsystem->SetInteractionPawn(GetPawn());
	}
}

void ACT_PlayerController::ApplyInputMode(ECTInputMode NewMode)
{
	CurrentInputMode = NewMode;

	switch (NewMode)
	{
	case ECTInputMode::Gameplay:
	{
		bShowMouseCursor = false;

		FInputModeGameOnly Mode;
		SetInputMode(Mode);

		SetIgnoreLookInput(false);
		break;
	}

	case ECTInputMode::GameplayWithCursor:
	{
		bShowMouseCursor = true;

		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(Mode);

		SetIgnoreLookInput(true);
		break;
	}

	case ECTInputMode::UIOnly:
	{
		bShowMouseCursor = true;

		FInputModeUIOnly Mode;
		SetInputMode(Mode);

		SetIgnoreLookInput(true);
		break;
	}
	}
}

void ACT_PlayerController::SetDeveloperCursorEnabled(bool bEnabled)
{
	if (bDeveloperCursorEnabled == bEnabled)
	{
		return;
	}

	bDeveloperCursorEnabled = bEnabled;


	ApplyInputMode(
		bEnabled
		? ECTInputMode::GameplayWithCursor
		: ECTInputMode::Gameplay);
}



void ACT_PlayerController::InitializeInputContexts()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!InputSubsystem)
	{
		return;
	}

	if (GameplayContext)
	{
		InputSubsystem->AddMappingContext(
			GameplayContext,
			GameplayMappingPriority);
	}

	UE_LOG(LogTemp, Display,
		TEXT("Gameplay Mapping Context applied."));
}

void ACT_PlayerController::AddInputLayer(ECTInputLayer Layer)
{

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();


	
	if (ActiveInputLayers.Contains(Layer))
	{
		return;
	}

	const FCTInputLayer* Entry = FindInputLayer(Layer);

	if (!Entry)
	{
		UE_LOG(LogCTInput, Warning,
			TEXT("Input Layer not found."));
		return;
	}

	InputSubsystem->AddMappingContext(
		Entry->MappingContext,
		Entry->Priority);

	ActiveInputLayers.Add(Layer);

	UE_LOG(LogCTInput, Display,
		TEXT("Added Input Layer: %s"),
		*UEnum::GetValueAsString(Layer));
}

	
void ACT_PlayerController::RemoveInputLayer(ECTInputLayer Layer)
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (ActiveInputLayers.Contains(Layer))
	{
		return;
	}

	const FCTInputLayer* Entry = FindInputLayer(Layer);

	if (!Entry)
	{
		return;
	}

	InputSubsystem->RemoveMappingContext(
		Entry->MappingContext);

	ActiveInputLayers.Remove(Layer);

	UE_LOG(LogCTInput, Display,
		TEXT("Removed Input Layer: %s"),
		*UEnum::GetValueAsString(Layer));

}

bool ACT_PlayerController::HasInputLayer(ECTInputLayer Layer) const
{
	
	if (ActiveInputLayers.Contains(Layer))
	{
		return true;
	}

	return false;
}

const FCTInputLayer* ACT_PlayerController::FindInputLayer(ECTInputLayer Layer) const
{
	
	for (const FCTInputLayer& Entry : InputLayers)
	{
		if (Entry.Layer == Layer)
		{
			return &Entry;
		}
	}

	return nullptr;
}

void ACT_PlayerController::ToggleDeveloperCursor()
{
	SetDeveloperCursorEnabled(!bDeveloperCursorEnabled);
}

void ACT_PlayerController::FadeScreen(bool bFadeToBlack,
	float Duration,
	bool bHoldWhenFinished)
{
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(
			bFadeToBlack ? 0.f : 1.f,
			bFadeToBlack ? 1.f : 0.f,
			Duration,
			FLinearColor::Black,
			false,
			bHoldWhenFinished
		);
	}
}

void ACT_PlayerController::TestResolver()
{
	UCT_InteractionSubsystem* InteractionSubsystem =
		GetGameInstance()->GetSubsystem<UCT_InteractionSubsystem>();

	if (!InteractionSubsystem)
	{
		return;
	}

	APawn* PlayerPawn = GetPawn();

	if (!PlayerPawn)
	{
		return;
	}

	UCT_InteractableComponent* Result =
		InteractionSubsystem->FindBestInteractable(PlayerPawn);

	if (!Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Interaction"));
		return;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("Best Interaction: %s"),
		*GetNameSafe(Result->GetOwner()));
}