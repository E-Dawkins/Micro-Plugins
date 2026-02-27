// Some copyright should be here...

using UnrealBuildTool;

public class MicroUnitConverter : ModuleRules
{
	public MicroUnitConverter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
                "Engine",
			}
		);
	}
}
