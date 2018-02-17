/*
 JSONSVGPlugin 0.0.1
 -----
 
*/
using UnrealBuildTool;

public class JSONSVGPlugin: ModuleRules
{
	public JSONSVGPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine"
            });

		PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Json",
                "JsonUtilities"
            });

		PrivateIncludePaths.Add("JSONSVGPlugin/Private");
	}
}
