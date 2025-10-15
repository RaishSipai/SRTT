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
			"UMG",
			// CRITICAL FIX: Adding the necessary Chaos Vehicles modules as per UE 5.6 requirements
			"ChaosVehicles",
			"ChaosVehiclesCore",
			"ChaosVehiclesEngine"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
