// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FireTeam : ModuleRules
{
	public FireTeam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "FireTeam" });

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
            "Networking", // 如果需要网络功能
			"Sockets"     // 如果需要网络功能
		});
	}
}
