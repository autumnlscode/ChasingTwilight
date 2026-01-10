#include "Subsystems/CT_SaveSubsystem.h"
#include "Data/CT_SaveGame.h"
#include "Subsystems/CT_TimeSubsystem.h"
#include "Subsystems/CT_FlagSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCT_SaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(UCT_TimeSubsystem::StaticClass());
	Collection.InitializeDependency(UCT_FlagSubsystem::StaticClass());

	TimeSubsystem = GetGameInstance()->GetSubsystem<UCT_TimeSubsystem>();
	FlagSubsystem = GetGameInstance()->GetSubsystem<UCT_FlagSubsystem>();

	if (TimeSubsystem)
	{
		TimeSubsystem->OnTimeUpdated.AddUObject(this, &UCT_SaveSubsystem::HandleTimeUpdated);
	}
	if (FlagSubsystem)
	{
		FlagSubsystem->OnFlagChanged.AddDynamic(this, &UCT_SaveSubsystem::HandleFlagChanged);
	}

	UE_LOG(LogTemp, Log, TEXT("[CT] SaveSubsystem Initialize (Slot=%s)"), *SlotName);
}

void UCT_SaveSubsystem::Deinitialize()
{
	if (TimeSubsystem)
	{
		TimeSubsystem->OnTimeUpdated.RemoveAll(this);
	}
	if (FlagSubsystem)
	{
		FlagSubsystem->OnFlagChanged.RemoveAll(this);
	}
	Super::Deinitialize();
}

void UCT_SaveSubsystem::SetActiveSlotName(FString NewSlotName)
{
	if (!NewSlotName.IsEmpty())
	{
		SlotName = MoveTemp(NewSlotName);
	}
}

void UCT_SaveSubsystem::SetDirty(bool bNewDirty)
{
	if (bDirty == bNewDirty) return;
	bDirty = bNewDirty;
	OnSaveDirtyChanged.Broadcast(bDirty);
}

void UCT_SaveSubsystem::HandleTimeUpdated(int32 /*NewDay*/, int32 /*NewMinutes*/, ECT_TimeBlock /*NewBlock*/)
{
	// v1: any time update marks dirty (you can narrow to block/day changes later)
	if (!bIsApplyingSave)
	{
		SetDirty(true);
	}
}

void UCT_SaveSubsystem::HandleFlagChanged(FGameplayTag /*Flag*/, bool /*bIsSet*/)
{
	SetDirty(true);
}

UCT_SaveGame* UCT_SaveSubsystem::BuildSaveObject() const
{
	UCT_SaveGame* SaveObj = Cast<UCT_SaveGame>(UGameplayStatics::CreateSaveGameObject(UCT_SaveGame::StaticClass()));
	if (!SaveObj) return nullptr;

	
	// Time
	if (TimeSubsystem)
	{
		SaveObj->DayNumber = TimeSubsystem->GetDayNumber();
		SaveObj->TimeMinutes = TimeSubsystem->GetTimeMinutes();
		SaveObj->Season = TimeSubsystem->GetSeason();
	}


	// Flags
	if (FlagSubsystem)
	{
		SaveObj->ActiveFlags = FlagSubsystem->GetAllFlags();
	}

	// Cycle (v1: pick from flags if you like)
	// If you're already using World.Cycle.* tags, this is a simple approach:
	if (FlagSubsystem)
	{
		// prefer the most "advanced" one if multiple ever exist
		const FGameplayTag NGpp = FGameplayTag::RequestGameplayTag(TEXT("World.Cycle.NGPlusPlus"), false);
		const FGameplayTag NGp = FGameplayTag::RequestGameplayTag(TEXT("World.Cycle.NGPlus"), false);
		const FGameplayTag NG = FGameplayTag::RequestGameplayTag(TEXT("World.Cycle.NG"), false);

		if (NGpp.IsValid() && FlagSubsystem->HasFlag(NGpp)) SaveObj->CycleTag = NGpp;
		else if (NGp.IsValid() && FlagSubsystem->HasFlag(NGp)) SaveObj->CycleTag = NGp;
		else if (NG.IsValid() && FlagSubsystem->HasFlag(NG)) SaveObj->CycleTag = NG;
	}

	return SaveObj;
}

void UCT_SaveSubsystem::ApplySaveObject(const UCT_SaveGame* SaveObj) const
{
	if (!SaveObj) return;

	// Time
	if (TimeSubsystem)
	{
		TimeSubsystem->SetDayNumber(SaveObj->DayNumber);
		TimeSubsystem->SetTimeMinutes(SaveObj->TimeMinutes);
		TimeSubsystem->SetSeason(SaveObj->Season);
	}

	// Flags
	if (FlagSubsystem)
	{
		FlagSubsystem->ClearAllFlags();
		for (const FGameplayTag& T : SaveObj->ActiveFlags)
		{
			FlagSubsystem->SetFlag(T, true);
		}
		// Also apply cycle tag if present
		if (SaveObj->CycleTag.IsValid())
		{
			FlagSubsystem->SetFlag(SaveObj->CycleTag, true);
		}
	}
}

bool UCT_SaveSubsystem::WriteSaveNow()
{
	UCT_SaveGame* SaveObj = BuildSaveObject();
	if (!SaveObj)
	{
		OnSaveWritten.Broadcast(false);
		return false;
	}

	const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex);
	if (bSuccess)
	{
		LastLoadedSave = SaveObj;
		SetDirty(false);
	}

	UE_LOG(LogTemp, Log, TEXT("[CT] Save write (%s): %s"), *SlotName, bSuccess ? TEXT("SUCCESS") : TEXT("FAIL"));
	OnSaveWritten.Broadcast(bSuccess);
	return bSuccess;
}


bool UCT_SaveSubsystem::LoadSaveNow()
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("[CT] Load requested but save slot missing: %s"), *SlotName);
		return false;
	}

	USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	UCT_SaveGame* SaveObj = Cast<UCT_SaveGame>(Loaded);
	if (!SaveObj)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CT] Failed to cast loaded save: %s"), *SlotName);
		return false;
	}

	LastLoadedSave = SaveObj;
	bIsApplyingSave = true;
	ApplySaveObject(SaveObj);
	bIsApplyingSave = false;
	SetDirty(false);

	UE_LOG(LogTemp, Log, TEXT("[CT] Save load (%s): SUCCESS"), *SlotName);
	OnSaveLoaded.Broadcast(SaveObj);
	return true;
}

bool UCT_SaveSubsystem::CreateNewSaveAndApply()
{
	UCT_SaveGame* SaveObj = BuildSaveObject();
	if (!SaveObj) return false;

	LastLoadedSave = SaveObj;
	ApplySaveObject(SaveObj);
	SetDirty(true); // new save should be dirty until written

	UE_LOG(LogTemp, Log, TEXT("[CT] New save created + applied (Slot=%s)"), *SlotName);
	return true;
}

UCT_SaveGame* UCT_SaveSubsystem::GetCurrentSnapshot() const
{
	return BuildSaveObject(); // creates a new save object with current Time/Flags/Cycle
}


