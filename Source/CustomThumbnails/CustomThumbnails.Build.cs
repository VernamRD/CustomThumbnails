// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomThumbnails : ModuleRules
{
	public CustomThumbnails(ReadOnlyTargetRules Target) : base(Target)
	{
		bUsePrecompiled = false;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"CoreUObject",
				"InputCore",
				"Slate",
				"EditorStyle",
				"AssetTools",
				"EditorWidgets",
				"UnrealEd",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"RenderCore",
				"CoreUObject",
				"Engine",
				"Projects",
				"AppFramework",
				"PropertyEditor",
				"EditorFramework",
				"Slate",
				"SlateCore",
				"InputCore",
				"ContentBrowser",
				"UnrealEd",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
