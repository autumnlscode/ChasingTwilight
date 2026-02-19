#include "Interaction/CT_InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "DrawDebugHelpers.h"

UCT_InteractionComponent::UCT_InteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCT_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	// Start periodic focus scanning
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TimerHandle_FocusScan,
			this,
			&UCT_InteractionComponent::UpdateFocus,
			ScanInterval,
			true
		);
	}

	UpdateFocus();
}

void UCT_InteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_FocusScan);
	}
	Super::EndPlay(EndPlayReason);
}

void UCT_InteractionComponent::AddCandidate(AActor* Target)
{
	if (!IsValid(Target)) return;

	AActor* OwnerActor = Owner.Get();
	if (Target == OwnerActor) return;

	// Prevent duplicates
	for (const TWeakObjectPtr<AActor>& W : Candidates)
	{
		if (W.Get() == Target)
		{
			return;
		}
	}

	Candidates.Add(Target);
}

void UCT_InteractionComponent::RemoveCandidate(AActor* Target)
{
	if (!IsValid(Target)) return;

	Candidates.RemoveAll([&](const TWeakObjectPtr<AActor>& W)
		{
			return W.Get() == Target;
		});

	// If we removed focus, clear it (or rescan immediately)
	if (FocusedActor.Get() == Target)
	{
		ApplyFocus(nullptr);
	}
}

void UCT_InteractionComponent::SetInteractionSuppressed(bool bSuppressed)
{
	bInteractionSuppressed = bSuppressed;

	if (bInteractionSuppressed)
	{
		ApplyFocus(nullptr);
	}
}

bool UCT_InteractionComponent::TryInteract(AActor* InInteractor, UObject*& OutTarget, AActor*& OutInteractor)
{
	OutInteractor = InInteractor;
	OutTarget = nullptr;

	if (!IsValid(InInteractor))
	{
		return false;
	}
	
	
	AActor* TargetActor = FocusedActor.Get();
	if (!IsValid(TargetActor))
	{
		return false;
	}

	OutTarget = TargetActor;

	// Phase 0.5: actually "do" the interact here (interface call, etc.)
	// For now we just report success + return the refs.
	return true;
}

void UCT_InteractionComponent::CleanupInvalidCandidates()
{
	Candidates.RemoveAll([](const TWeakObjectPtr<AActor>& W)
		{
			return !W.IsValid();
		});
}

bool UCT_InteractionComponent::IsCandidate(AActor* InActor) const
{
	if (!InActor) return false;
	for (const TWeakObjectPtr<AActor>& W : Candidates)
	{
		if (W.Get() == InActor) return true;
	}
	return false;
}

AActor* UCT_InteractionComponent::GetTraceHitActor(bool& bOutHit) const
{
	bOutHit = false;

	UWorld* World = GetWorld();
	if (!World) return nullptr;

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC || !PC->PlayerCameraManager) return nullptr;

	const FVector Start = PC->PlayerCameraManager->GetCameraLocation();
	const FVector Forward = PC->PlayerCameraManager->GetCameraRotation().Vector();
	const FVector End = Start + (Forward * MaxInteractDistance);

	// store debug vectors (mutable members, so cast away const if you keep this const)
	// If you want this to remain const, remove these or make them mutable.
	const_cast<UCT_InteractionComponent*>(this)->ViewStart = Start;
	const_cast<UCT_InteractionComponent*>(this)->ViewForward = Forward;
	const_cast<UCT_InteractionComponent*>(this)->ViewEnd = End;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(CT_InteractTrace), false);
	if (AActor* OwnerActor = Owner.Get())
	{
		Params.AddIgnoredActor(OwnerActor);
	}

	FHitResult Hit;
	const bool bHit = World->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(TraceRadius),
		Params
	);

	bOutHit = bHit;

	if (bDebugDraw && DrawDebugType != EDrawDebugTrace::None)
	{
		const FColor Color = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(World, Start, End, Color, false, ScanInterval, 0, 1.5f);
		DrawDebugSphere(World, bHit ? Hit.ImpactPoint : End, TraceRadius, 12, Color, false, ScanInterval);
	}

	return bHit ? Hit.GetActor() : nullptr;
}

void UCT_InteractionComponent::ApplyFocus(AActor* NewFocus)
{
	AActor* Old = FocusedActor.Get();
	if (Old == NewFocus) return;

	FocusedActor = NewFocus;
	OnFocusedActorChanged.Broadcast(Old, NewFocus);
}

void UCT_InteractionComponent::UpdateFocus()
{
	if (bInteractionSuppressed)
	{
		return;
	}

	CleanupInvalidCandidates();

	bool bTraceHit = false;
	AActor* HitActor = GetTraceHitActor(bTraceHit);

	// Prefer trace if allowed
	if (bTraceHit && IsValid(HitActor))
	{
		if (!bRequiredCandidateList || IsCandidate(HitActor))
		{
			ApplyFocus(HitActor);
			return;
		}
	}

	// Otherwise score candidates
	AActor* BestActor = nullptr;
	float BestScore = -FLT_MAX;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC || !PC->PlayerCameraManager)
	{
		ApplyFocus(nullptr);
		return;
	}

	const FVector Start = ViewStart;      // already set by trace
	const FVector Forward = ViewForward;  // already set by trace

	for (const TWeakObjectPtr<AActor>& W : Candidates)
	{
		AActor* A = W.Get();
		if (!IsValid(A)) continue;

		const FVector To = (A->GetActorLocation() - Start);
		const float Dist = To.Size();
		if (Dist > MaxInteractDistance) continue;

		const FVector Dir = To.GetSafeNormal();
		const float Dot = FVector::DotProduct(Forward, Dir);
		if (Dot < MinDot) continue;

		const float Score = (Dot * 2.f) - (Dist / MaxInteractDistance);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestActor = A;
		}
	}

	ApplyFocus(BestActor);
}
