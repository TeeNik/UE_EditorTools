// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorToolsEditor : ModuleRules
{
    public EditorToolsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "EditorTools",
                "PropertyEditor",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "GraphEditor",
                "BlueprintGraph",
                "Slate",
                "SlateCore",
                "EditorTools",
                "ComponentVisualizers",
                "GameplayTags",
                "PropertyEditor",
            }
        );
    }
}