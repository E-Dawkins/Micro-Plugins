// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MicroPathfinder3dEditor : ModuleRules
{
	public MicroPathfinder3dEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",				// required for component visualizers
				"Slate",
				"SlateCore"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"MicroPathfinder3d",	// runtime module
				"InputCore",
				"EditorFramework",
				"LevelEditor",
				"PropertyEditor",
				"RenderCore",			// for DrawWireBox, etc.
				"RHI",
				"SceneOutliner"
			}
		);
	}
}
