

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CT_DiscoveryTypes.h"
#include "CT_DiscoveryActor.generated.h"

UCLASS()
class CHASINGTWILIGHT_API ACT_DiscoveryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACT_DiscoveryActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Discovery")
	FName DiscoveryID;

	UFUNCTION(BlueprintCallable, Category="Discovery")
	bool Discover();
	

protected:
	
	virtual void BeginPlay() override;

	virtual FCTDiscoveryRequest CreateDiscoveryRequest() const;

public:	
	
	virtual void Tick(float DeltaTime) override;

	

};
