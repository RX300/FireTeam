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
            "Networking", // �����Ҫ���繦��
			"Sockets"     // �����Ҫ���繦��
		});
	}
}
