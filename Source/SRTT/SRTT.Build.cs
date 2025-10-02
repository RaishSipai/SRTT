// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SRTT : ModuleRules
{
	public SRTT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore", 			// Required for input handling
			"EnhancedInput", 		// The modern input system we are using
			"PhysicsCore",			// Core physics types
			"ChaosVehicles",		// For the Chaos Vehicle physics simulation
			"UMG",					// For the User Interface (Unreal Motion Graphics)
			"AIModule",				// For AI controllers and Behavior Trees
			"GameplayTasks",		// Required for certain AI functionalities
			"NavigationSystem",		// For AI pathfinding
			"NetCore",				// Core networking types
			"OnlineSubsystem",		// The base classes for online services like Steam or EOS
			"OnlineSubsystemEOS"	// Specifically for Epic Online Services
		});


        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
