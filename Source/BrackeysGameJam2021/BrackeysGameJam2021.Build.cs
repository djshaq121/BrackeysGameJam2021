// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BrackeysGameJam2021 : ModuleRules
{
	public BrackeysGameJam2021(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
