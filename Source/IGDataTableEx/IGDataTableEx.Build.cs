// Copyright Ian Good

using UnrealBuildTool;

public class IGDataTableEx : ModuleRules
{
	public IGDataTableEx(ReadOnlyTargetRules Target) : base(Target)
	{
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Error;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
		});
	}
}
