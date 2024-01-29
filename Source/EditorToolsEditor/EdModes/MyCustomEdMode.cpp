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

bool FMyCustomEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag,
	FRotator& InRot, FVector& InScale)
{
	if (InRot.IsNearlyZero())
	{
		return false;
	}
	
	auto ViewTransform = InViewportClient->GetViewTransform();
	FTransform Transform(ViewTransform.GetRotation(), ViewTransform.GetLocation());

	FQuat NewQuat = ViewTransform.GetRotation().Quaternion() * FQuat(0,1,0,InRot.Yaw);

	FRotator Roll(0, 0, 90);
	Transform.Rotator() = Roll;
	FRotator TestRot = Transform.InverseTransformRotation(FRotator(0, 10, 0).Quaternion()).Rotator();
	TestRot = (Roll.Quaternion() * FQuat(FVector::UpVector, FMath::DegreesToRadians(10))).Rotator();
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::TestRot %s %s"), *TestRot.ToString(), *Roll.Quaternion().ToString());

	FRotator FinalRot = (ViewTransform.GetRotation().Quaternion() * FQuat(FVector::UpVector, FMath::DegreesToRadians(InRot.Yaw))).Rotator();
	FRotator DeltaRot = FinalRot - ViewTransform.GetRotation();
	DeltaRot.Normalize();
	
	//InViewportClient->SetViewRotation(FinalRot);
	
	//FRotator NewRot(InRot.Pitch, InRot.Roll, InRot.Yaw);
	InViewportClient->PeformDefaultCameraMovement(InDrag, DeltaRot, InScale);

	FinalRot = (ViewTransform.GetRotation().Quaternion() * FQuat(FVector::LeftVector, FMath::DegreesToRadians(InRot.Pitch))).Rotator();
	DeltaRot = FinalRot - ViewTransform.GetRotation();
	DeltaRot.Normalize();
	InViewportClient->PeformDefaultCameraMovement(InDrag, DeltaRot, InScale);

	
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::InputDelta %s %s"), *InDrag.ToString(), *InRot.ToString());
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::Transform %s %s"), *ViewTransform.GetRotation().ToString(), *Transform.GetRotation().Rotator().ToString());
	return true;
}

bool FMyCustomEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Event == IE_Repeat && Key == EKeys::E)
	{
		FVector Zero = FVector::Zero();
		FRotator DeltaRoll = FRotator(0,0,FMath::DegreesToRadians(100));
		ViewportClient->PeformDefaultCameraMovement(Zero, DeltaRoll, Zero);
		UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::InputKey"));
		return true;
	}
	if (Event == IE_Repeat && Key == EKeys::Q)
	{
		FVector Zero = FVector::Zero();
		FRotator DeltaRoll = FRotator(0,0,FMath::DegreesToRadians(-100));
		ViewportClient->PeformDefaultCameraMovement(Zero, DeltaRoll, Zero);
		UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::InputKey"));
		return true;
	}
	if (Event == IE_Pressed && Key == EKeys::R)
	{
		ViewportClient->SetViewRotation(FRotator::ZeroRotator);
		return true;
	}
	return false;
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
