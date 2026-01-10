#include "Subsystems/CT_DialogueSubsystem.h"
#include "Dialogue/CT_DialogueDataAsset.h"

#include "Subsystems/CT_FlagSubsystem.h"
#include "Subsystems/CT_TimeSubsystem.h"
#include "Subsystems/CT_SubsystemManagerSubsystem.h"

void UCT_DialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(UCT_TimeSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_FlagSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_SubsystemManagerSubsystem::StaticClass());

	TimeSubsystem = GetGameInstance()->GetSubsystem<UCT_TimeSubsystem>();
	FlagSubsystem = GetGameInstance()->GetSubsystem<UCT_FlagSubsystem>();
	SubsystemManager = GetGameInstance()->GetSubsystem<UCT_SubsystemManagerSubsystem>();
}

void UCT_DialogueSubsystem::Deinitialize()
{
	TimeSubsystem = nullptr;
	FlagSubsystem = nullptr;
	SubsystemManager = nullptr;

	Super::Deinitialize();
}

bool UCT_DialogueSubsystem::StartDialogue(UCT_DialogueDataAsset* DialogueAsset, UObject* Speaker)
{
	if (!DialogueAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CT][DIA] StartDialogue failed: null asset"));
		return false;
	}

	Session.Asset = DialogueAsset;
	Session.Speaker = Speaker;
	Session.CurrentNodeId = DialogueAsset->StartNodeId;
	Session.bInDialogue = true;

	Advance(); // Let Advance do the actual evaluation/speaking
	return true;
}

void UCT_DialogueSubsystem::Advance()
{
	if (!Session.bInDialogue || !Session.Asset)
	{
		return;
	}

	if (Session.CurrentNodeId.IsNone())
	{
		EndDialogue();
		return;
	}

	FCT_DialogueNode Node;
	if (!GetNodeById(Session.Asset, Session.CurrentNodeId, Node))
	{
		UE_LOG(LogTemp, Warning, TEXT("[CT][DIA] Advance failed: node not found (%s)"), *Session.CurrentNodeId.ToString());
		EndDialogue();
		return;
	}

	SpeakNodeAndAdvanceState(Node);
}

void UCT_DialogueSubsystem::EndDialogue()
{
	if (!Session.bInDialogue)
	{
		return;
	}

	Session.bInDialogue = false;
	Session.Asset = nullptr;
	Session.Speaker.Reset();
	Session.CurrentNodeId = NAME_None;

	LastSpokenLine = FText::GetEmpty();

	OnDialogueEnded.Broadcast();
}

bool UCT_DialogueSubsystem::FindNodeIndexById(const UCT_DialogueDataAsset* Asset, FName NodeId, int32& OutIndex) const
{
	OutIndex = INDEX_NONE;
	if (!Asset || NodeId.IsNone()) return false;

	for (int32 i = 0; i < Asset->Nodes.Num(); ++i)
	{
		if (Asset->Nodes[i].NodeId == NodeId)
		{
			OutIndex = i;
			return true;
		}
	}
	return false;
}

bool UCT_DialogueSubsystem::FindNodeById(const UCT_DialogueDataAsset* Asset, FName NodeId, FCT_DialogueNode& OutNode) const
{
	if (!Asset || NodeId.IsNone())
	{
		return false;
	}

	for (const FCT_DialogueNode& Node : Asset->Nodes)
	{
		if (Node.NodeId == NodeId)
		{
			OutNode = Node; // copy out for BP friendliness
			return true;
		}
	}

	return false;
}

bool UCT_DialogueSubsystem::GetNodeById(const UCT_DialogueDataAsset* Asset, FName NodeId, FCT_DialogueNode& OutNode) const
{
	int32 Index = INDEX_NONE;
	if (!FindNodeIndexById(Asset, NodeId, Index)) return false;

	OutNode = Asset->Nodes[Index];
	return true;
}

FText UCT_DialogueSubsystem::GetLastSpokenLine() const
{
	return LastSpokenLine;
}


bool UCT_DialogueSubsystem::EvaluateNode(const FCT_DialogueNode& Node, FString& OutFailReason) const
{
	if (Node.ConditionGroups.Num() == 0)
	{
		OutFailReason.Reset(); // important
		return true;
	}

	for (const FCT_ConditionGroup& Group : Node.ConditionGroups)
	{
		FString GroupFail;
		if (!SubsystemManager->CT_EvaluateConditionGroup(Group, GroupFail))
		{
			OutFailReason = GroupFail;
			return false;
		}
	}

	OutFailReason.Reset();
	return true;
}


void UCT_DialogueSubsystem::SpeakNodeAndAdvanceState(const FCT_DialogueNode& StartNode)
{
	if (!Session.Asset) { EndDialogue(); return; }

	FCT_DialogueNode Node = StartNode;

	// Prevent infinite fallback loops
	int32 Safety = 0;
	const int32 MaxHops = 8;

	while (Safety++ < MaxHops)
	{
		FString FailReason;
		const bool bPass = EvaluateNode(Node, FailReason);

		if (bPass)
		{
			ApplyNodeSideEffects(Node);

			LastSpokenLine = Node.LineText;
			OnLineSpoken.Broadcast(Node.LineText);

			Session.CurrentNodeId = Node.NextNodeId;
			if (Session.CurrentNodeId.IsNone())
			{
				EndDialogue();
			}
			return;
		}

		// Blocked: try fallback
		if (!Node.FallbackNodeId.IsNone())
		{
			FCT_DialogueNode FallbackNode;
			if (FindNodeById(Session.Asset, Node.FallbackNodeId, FallbackNode))
			{
				Session.CurrentNodeId = Node.FallbackNodeId;
				// Speak fallback immediately (but avoid infinite loops)
				SpeakNodeAndAdvanceState(FallbackNode);
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("[CT][DIA] Fallback node not found: %s"), *Node.FallbackNodeId.ToString());
		}

		// No fallback, end
		EndDialogue();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[CT][DIA] Fallback loop detected. Ending dialogue."));
	EndDialogue();
}


void UCT_DialogueSubsystem::ApplyNodeSideEffects(const FCT_DialogueNode& Node)
{
	if (!FlagSubsystem) return;

	for (const FCT_DialogueSideEffect& FX : Node.SideEffects)
	{
		if (!FX.Tag.IsValid()) continue;

		switch (FX.Type)
		{
		case ECT_DialogueSideEffectType::AddFlagTag:
			FlagSubsystem->SetFlag(FX.Tag, true);
			break;

		case ECT_DialogueSideEffectType::RemoveFlagTag:
			FlagSubsystem->SetFlag(FX.Tag, false);
			break;

		default:
			break;
		}
	}
}

