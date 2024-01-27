// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomEdMode.h"

#define LOCTEXT_NAMESPACE "FSampleToolsEditorMode"

const FEditorModeID FMyCustomEdMode::EM_MyCustomEditorModeId = TEXT("EM_MyCustomEditorMode");

FMyCustomEdMode::FMyCustomEdMode()
{
	Info = FEditorModeInfo(FMyCustomEdMode::EM_MyCustomEditorModeId,
		LOCTEXT("SMyCustomEditorModeName", "MyCustomEditorMode"),
		FSlateIcon(),
		true);
}


FMyCustomEdMode::~FMyCustomEdMode()
{
}


void FMyCustomEdMode::ActorSelectionChangeNotify()
{
	// @todo support selection change
}

void FMyCustomEdMode::Enter()
{
	FEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	//const FSampleToolsEditorModeCommands& SampleToolCommands = FSampleToolsEditorModeCommands::Get();

	//auto CreateActorSampleToolBuilder = NewObject< UCreateActorSampleToolBuilder>(this);
	//RegisterTool(SampleToolCommands.CreateActorTool, TEXT("CreateActorSampleTool"), CreateActorSampleToolBuilder);

	//RegisterTool(SampleToolCommands.DrawCurveOnMeshTool, TEXT("DrawCurveOnMeshSampleTool"), NewObject<UDrawCurveOnMeshSampleToolBuilder>(this));
	//RegisterTool(SampleToolCommands.MeasureDistanceTool, TEXT("MeasureDistanceSampleTool"), NewObject<UMeasureDistanceSampleToolBuilder>(this));
	//RegisterTool(SampleToolCommands.SurfacePointTool, TEXT("SurfacePointTool"), NewObject<UMeshSurfacePointToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	//GetToolManager()->SelectActiveToolType(EToolSide::Left, TEXT("SurfacePointTool"));
}

#undef LOCTEXT_NAMESPACE
