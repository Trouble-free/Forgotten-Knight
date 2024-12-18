// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MeleeCombat : ModuleRules
{
	public MeleeCombat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
			"UMG",
			"AIModule",
            "NavigationSystem",
            "GameplayTasks",
			"Slate",
			"SlateCore"
        });
	}
}
