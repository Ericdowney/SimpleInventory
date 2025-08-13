using UnrealBuildTool;

public class SimpleInventoryTests : ModuleRules
{
    public SimpleInventoryTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "SimpleInventory",
            "AutomationTest"
        });
        
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "UnrealEd",
                "EditorSubsystem"
            });

            PublicIncludePaths.AddRange(new string[]
            {
                "Editor/UnrealEd/Public"
            });
        }
    }
}
