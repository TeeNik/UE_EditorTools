#include "EditorToolsEditor.h"
#include "EditorTools/WorldObjects/SpotComponent.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "Utils/ClassSelectorCustomization.h"
#include "Utils/SpotComponentVisualizer.h"

DEFINE_LOG_CATEGORY(EditorToolsEditor)

#define LOCTEXT_NAMESPACE "FEditorToolsEditorModule"

void FEditorToolsEditorModule::StartupModule()
{
    UE_LOG(EditorToolsEditor, Warning, TEXT("FEditorToolsEditor: Log Started"));

	if (GUnrealEd)
	{
		TSharedPtr<FSpotComponentVisualizer> Visualizer = MakeShareable(new FSpotComponentVisualizer());
		GUnrealEd->RegisterComponentVisualizer(USpotComponent::StaticClass()->GetFName(), Visualizer);
	}

	// Register the details customizer
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("ClassSelector", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FClassSelectorCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FEditorToolsEditorModule::ShutdownModule()
{
    UE_LOG(EditorToolsEditor, Warning, TEXT("FEditorToolsEditor: Log Ended"));

	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(USpotComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorToolsEditorModule, EditorToolsEditor)