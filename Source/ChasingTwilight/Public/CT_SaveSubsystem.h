#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "CT_TimeSubsystem.h" // for ECT_TimeBlock
#include "CT_SaveSubsystem.generated.h"

class UCT_SaveGame;
class UCT_TimeSubsystem;
class UCT_FlagSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCT_OnSaveLoaded, UCT_SaveGame*, SaveObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCT_OnSaveWritten, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCT_OnSaveDirtyChanged, bool, bIsDirty);

UCLASS()
class CHASINGTWILIGHT_API UCT_SaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- Save Slot ---
	UFUNCTION(BlueprintCallable, Category = "CT|Save")
	void SetActiveSlotName(FString NewSlotName);

	UFUNCTION(BlueprintPure, Category = "CT|Save")
	FString GetActiveSlotName() const { return SlotName; }

	// --- Operations ---
	UFUNCTION(BlueprintCallable, Category = "CT|Save")
	bool WriteSaveNow();

	UFUNCTION(BlueprintCallable, Category = "CT|Save")
	bool LoadSaveNow();

	UFUNCTION(BlueprintCallable, Category = "CT|Save")
	bool CreateNewSaveAndApply();

	// --- State ---
	UFUNCTION(BlueprintPure, Category = "CT|Save")
	bool IsDirty() const { return bDirty; }

	UFUNCTION(BlueprintCallable, Category = "CT|Save")
	void SetDirty(bool bNewDirty);

	UFUNCTION(BlueprintPure, Category = "CT|Save|Debug")
	UCT_SaveGame* GetCurrentSnapshot() const;

	UFUNCTION(BlueprintPure, Category = "CT|Save|Debug")
	UCT_SaveGame* GetLastLoadedSave() const { return LastLoadedSave; }

	UPROPERTY(BlueprintAssignable, Category = "CT|Save")
	FCT_OnSaveLoaded OnSaveLoaded;

	UPROPERTY(BlueprintAssignable, Category = "CT|Save")
	FCT_OnSaveWritten OnSaveWritten;

	UPROPERTY(BlueprintAssignable, Category = "CT|Save")
	FCT_OnSaveDirtyChanged OnSaveDirtyChanged;

private:
	UCT_SaveGame* BuildSaveObject() const;
	void ApplySaveObject(const UCT_SaveGame* SaveObj) const;

	void HandleTimeUpdated(int32 NewDay, int32 NewMinutes, ECT_TimeBlock NewBlock);
	void HandleFlagChanged(FGameplayTag Flag, bool bIsSet);

private:
	UPROPERTY()
	TObjectPtr<UCT_TimeSubsystem> TimeSubsystem;

	UPROPERTY()
	TObjectPtr<UCT_FlagSubsystem> FlagSubsystem;

	UPROPERTY()
	TObjectPtr<UCT_SaveGame> LastLoadedSave;

	UPROPERTY()
	FString SlotName = TEXT("CT_Autosave");

	UPROPERTY()
	int32 UserIndex = 0;

	UPROPERTY()
	bool bDirty = false;

	UPROPERTY()
	bool bIsApplyingSave = false;

};
