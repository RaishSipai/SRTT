// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SRTT : ModuleRules
{
	public SRTT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "ChaosVehicles",
            "UMG",
            "Slate",
            "SlateCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

		PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AIModule",
            "NavigationSystem"
        });
	}
}
