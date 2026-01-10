#include "Core/CT_GameInstance.h"
#include "Subsystems/CT_GameFlowSubsystem.h"   // <-- needed for UCT_GameFlowSubsystem
#include "Data/CT_GameFlowTypes.h"       // <-- needed for ECT_GameFlowState (sometimes included via subsystem, but safe)
#include "Engine/Engine.h"


void UCT_GameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Log, TEXT("[CT] GameInstance Init"));

	if (UCT_GameFlowSubsystem* Flow = GetSubsystem<UCT_GameFlowSubsystem>())
	{
		Flow->SetState(ECT_GameFlowState::FrontEnd); // or InGame for now
	}
}

void UCT_GameInstance::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("[CT] GameInstance Shutdown"));
	Super::Shutdown();
}
