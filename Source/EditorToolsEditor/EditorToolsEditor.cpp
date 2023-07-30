#include "EditorToolsEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

DEFINE_LOG_CATEGORY(EditorToolsEditor)

#define LOCTEXT_NAMESPACE "FEditorToolsEditorModule"

void FEditorToolsEditorModule::StartupModule()
{
    UE_LOG(EditorToolsEditor, Warning, TEXT("FEditorToolsEditor: Log Started"));
}

void FEditorToolsEditorModule::ShutdownModule()
{
    UE_LOG(EditorToolsEditor, Warning, TEXT("FEditorToolsEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorToolsEditorModule, EditorToolsEditor)