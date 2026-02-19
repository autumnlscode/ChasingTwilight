#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h" // <-- this defines EDrawDebugTrace
#include "CT_InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCT_OnFocusedActorChanged, AActor*, OldActor, AActor*, NewActor);

UCLASS(ClassGroup = (CT), meta = (BlueprintSpawnableComponent))
class CHASINGTWILIGHT_API UCT_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCT_InteractionComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// --- Candidates ---
	UFUNCTION(BlueprintCallable, Category = "CT|Interact")
	void AddCandidate(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "CT|Interact")
	void RemoveCandidate(AActor* Target);

	// --- Focus ---
	UFUNCTION(BlueprintCallable, Category = "CT|Interact")
	void UpdateFocus();

	UFUNCTION(BlueprintCallable, Category = "CT|Interact")
	void SetInteractionSuppressed(bool bSuppressed);

	UFUNCTION(BlueprintPure, Category = "CT|Interact")
	AActor* GetFocusedActor() const { return FocusedActor.Get(); }

	// --- Interaction ---
	UFUNCTION(BlueprintCallable, Category = "CT|Interact")
	bool TryInteract(AActor* InInteractor, UObject*& OutTarget, AActor*& OutInteractor);

	UPROPERTY(BlueprintAssignable, Category = "CT|Interact")
	FCT_OnFocusedActorChanged OnFocusedActorChanged;

	// ====== Tunables (your BP variables) ======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning")
	bool bRequiredCandidateList = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning")
	bool bInteractionSuppressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Debug")
	bool bDebugDraw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning", meta = (ClampMin = "0.01"))
	float ScanInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning", meta = (ClampMin = "1.0"))
	float MaxInteractDistance = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning", meta = (ClampMin = "0.0"))
	float TraceRadius = 18.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float MinDot = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CT|Interact|Tuning")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	// Optional: keep these as visible debug values like you had
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Interact|Debug")
	FVector ViewStart = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Interact|Debug")
	FVector ViewForward = FVector::ForwardVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CT|Interact|Debug")
	FVector ViewEnd = FVector::ZeroVector;

private:
	// Your BP “Candidates array”
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> Candidates;

	// Your BP “Focused Actor”
	UPROPERTY()
	TWeakObjectPtr<AActor> FocusedActor;

	// Your BP “Owner”
	UPROPERTY()
	TWeakObjectPtr<AActor> Owner;

	// TimerHandle_FocusScan
	FTimerHandle TimerHandle_FocusScan;

private:
	void ApplyFocus(AActor* NewFocus);
	void CleanupInvalidCandidates();
	bool IsCandidate(AActor* InActor) const;
	AActor* GetTraceHitActor(bool& bOutHit) const;
};
