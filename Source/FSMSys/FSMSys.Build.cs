// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FSMSys : ModuleRules
{
	public FSMSys(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealStateMachine" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UnrealStateMachine", "AIModule", "NavigationSystem" });
	}
}
