#include "EditorToolsEditor.h"
#include "ISettingsModule.h"
#include "EditorTools/WorldObjects/SpotComponent.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "Subsystems/ActorReplacementSettings.h"
#include "Utils/ClassSelectorCustomization.h"
#include "Utils/SpotComponentVisualizer.h"

DEFINE_LOG_CATEGORY(EditorToolsEditor)

#define LOCTEXT_NAMESPACE "FEditorToolsEditorModule"

void FEditorToolsEditorModule::StartupModule()
{
    UE_LOG(EditorToolsEditor, Warning, TEXT("FEditorToolsEditor: Log Started"));

	RegisterComponentVisualizers();

	// Register the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("ClassSelector", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FClassSelectorCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",
			"EditorTools",
			"Actor Replacement Settings",
			LOCTEXT("ActorReplacementSettings", "Actor Replacement Settings"),
			LOCTEXT("ActorReplacementSettingsDescription", "Configure Actor Replacement Settings"),
			GetMutableDefault<UActorReplacementSettings>());
	}
}

void FEditorToolsEditorModule::ShutdownModule()
{
    UE_LOG(EditorToolsEditor, Warning, TEXT("FEditorToolsEditor: Log Ended"));

	UnregisterComponentVisualizers();
}

void FEditorToolsEditorModule::RegisterComponentVisualizers()
{
	if (GUnrealEd)
	{
		RegisterComponentVisualizer(USpotComponent::StaticClass()->GetFName(), MakeShareable(new FSpotComponentVisualizer));
	}
}

void FEditorToolsEditorModule::UnregisterComponentVisualizers()
{
	if (GUnrealEd)
	{
		for (const FName ClassName : RegisteredComponentClassNames)
		{
			GUnrealEd->UnregisterComponentVisualizer(ClassName);
		}
		RegisteredComponentClassNames.Empty();
	}
}

void FEditorToolsEditorModule::RegisterComponentVisualizer(FName ComponentClassName, TSharedPtr<FComponentVisualizer> Visualizer)
{
	if (GUnrealEd && Visualizer.IsValid())
	{
		GUnrealEd->RegisterComponentVisualizer(ComponentClassName, Visualizer);
		Visualizer->OnRegister();

		RegisteredComponentClassNames.Add(ComponentClassName);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorToolsEditorModule, EditorToolsEditor)