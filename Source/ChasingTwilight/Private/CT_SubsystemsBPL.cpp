// CT_SubsystemsBPL.cpp

#include "CT_SubsystemsBPL.h"

#include "CT_SubsystemManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

UCT_SubsystemManagerSubsystem* UCT_SubsystemsBPL::CT_GetSubsystemManager(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GI)
	{
		return nullptr;
	}

	return GI->GetSubsystem<UCT_SubsystemManagerSubsystem>();
}
