// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChasingTwilight : ModuleRules
{
	public ChasingTwilight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
            "CommonUI",
            "Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
            "GameplayTags",
            "UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ChasingTwilight",
			"ChasingTwilight/Variant_Platforming",
			"ChasingTwilight/Variant_Platforming/Animation",
			"ChasingTwilight/Variant_Combat",
			"ChasingTwilight/Variant_Combat/AI",
			"ChasingTwilight/Variant_Combat/Animation",
			"ChasingTwilight/Variant_Combat/Gameplay",
			"ChasingTwilight/Variant_Combat/Interfaces",
			"ChasingTwilight/Variant_Combat/UI",
			"ChasingTwilight/Variant_SideScrolling",
			"ChasingTwilight/Variant_SideScrolling/AI",
			"ChasingTwilight/Variant_SideScrolling/Gameplay",
			"ChasingTwilight/Variant_SideScrolling/Interfaces",
			"ChasingTwilight/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
