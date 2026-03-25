// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AsyncTickPhysics : ModuleRules
{
	public AsyncTickPhysics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Chaos",
				"PhysicsCore",
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// Runtime module only. Keep private deps minimal so public headers remain usable from game modules.
			}
			);

		SetupModulePhysicsSupport(Target);
	}
}
