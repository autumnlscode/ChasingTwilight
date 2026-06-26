// Fill out your copyright notice in the Description page of Project Settings.


#include "Exploration/CT_DiscoveryDatabase.h"
#include "Exploration/CT_DiscoveryDefinition.h"




UCT_DiscoveryDefinition* UCT_DiscoveryDatabase::FindDefinition(FName DiscoveryID) const
{

	UE_LOG(LogTemp, Display,
		TEXT("Looking for: %s"),
		*DiscoveryID.ToString());

	UE_LOG(LogTemp, Display,
		TEXT("Database contains %d definitions"),
		Discoveries.Num());


	for (UCT_DiscoveryDefinition* Definition : Discoveries)
	{
		if (!Definition)
		{
			continue;
		}

		if (Definition->DiscoveryID == DiscoveryID)
		{
			return Definition;
		}

		UE_LOG(LogTemp, Display,
			TEXT("Checking: %s"),
			*Definition->DiscoveryID.ToString());
	}

	return nullptr;
}