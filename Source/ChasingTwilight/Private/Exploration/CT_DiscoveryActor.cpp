

#include "Exploration/CT_DiscoveryActor.h"
#include "Subsystems/CT_ExplorationSubsystem.h"
#include "Engine/GameInstance.h"


ACT_DiscoveryActor::ACT_DiscoveryActor()
{

	PrimaryActorTick.bCanEverTick = true;

}


void ACT_DiscoveryActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACT_DiscoveryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACT_DiscoveryActor::Discover()
{
	if (!GetGameInstance())
	{
		return false;
	}

	if (!DiscoveryDefinition)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("%s has no DiscoveryDefinitiion assigned."),
			*GetName());
		return false;
	}


	UCT_ExplorationSubsystem* Exploration =
		GetGameInstance()->GetSubsystem<UCT_ExplorationSubsystem>();

	if (!Exploration)
	{
		return false;
	}


	
	return Exploration->Discover(CreateDiscoveryRequest());
		
}




FCTDiscoveryRequest ACT_DiscoveryActor::CreateDiscoveryRequest() const
{
	FCTDiscoveryRequest Request;

	Request.Definition = DiscoveryDefinition;
	Request.WorldLocation = GetActorLocation();
	Request.Method = ECTDiscoveryMethod::Interaction;

	return Request;
}