// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomEdMode.h"

#include "Kismet/KismetMathLibrary.h"

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

bool FMyCustomEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	//TODO check RMB pressed

	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::CameraRot %s"), *InViewportClient->GetViewRotation().ToString());
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::InputDelta %s %s"), *InDrag.ToString(), *InRot.ToString());

	if (bDisable)
	{
		return false;
	}

	if (InRot.IsNearlyZero())
	{
		return false;
	}

	

	FRotator ViewRotation = InViewportClient->GetViewRotation();

	ViewRotation = (GravityToWorldTransform * ViewRotation.Quaternion()).Rotator();
	
	ViewRotation += FRotator( InRot.Pitch, InRot.Yaw, InRot.Roll );
	// normalize to -180 to 180
	ViewRotation.Pitch = FRotator::NormalizeAxis(ViewRotation.Pitch);
	// Make sure its withing  +/- 90 degrees (minus a small tolerance to avoid numerical issues w/ camera orientation conversions later on).
	ViewRotation.Pitch = FMath::Clamp( ViewRotation.Pitch, -90.f+KINDA_SMALL_NUMBER, 90.f-KINDA_SMALL_NUMBER );

	ViewRotation = (WorldToGravityTransform * ViewRotation.Quaternion()).Rotator();

	
	//ViewRotation = (ViewRotation.Quaternion() * FQuat(FVector::UpVector, FMath::DegreesToRadians(InRot.Yaw))).Rotator();
	//InViewportClient->SetViewRotation(ViewRotation);

	//ViewRotation = InViewportClient->GetViewRotation();
	//ViewRotation = (ViewRotation.Quaternion() * FQuat(FVector::RightVector, FMath::DegreesToRadians(-InRot.Pitch))).Rotator();

	// normalize to -180 to 180
	//ViewRotation.Yaw = FRotator::NormalizeAxis(ViewRotation.Yaw);
	//ViewRotation.Yaw = FMath::Clamp(ViewRotation.Yaw, 0.f+KINDA_SMALL_NUMBER, 175.f-KINDA_SMALL_NUMBER);
	InViewportClient->SetViewRotation(ViewRotation);
	
	//InViewportClient->PeformDefaultCameraMovement(InDrag, InRot, InScale);

	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::CameraRot %s"), *InViewportClient->GetViewRotation().ToString());
	
	return true;
	
	auto ViewTransform = InViewportClient->GetViewTransform();
	FTransform Transform(ViewTransform.GetRotation(), ViewTransform.GetLocation());

	FQuat NewQuat = ViewTransform.GetRotation().Quaternion() * FQuat(0,1,0,InRot.Yaw);

	FRotator Roll(0, 0, 90);
	Transform.Rotator() = Roll;
	FRotator TestRot = Transform.InverseTransformRotation(FRotator(0, 10, 0).Quaternion()).Rotator();
	TestRot = (Roll.Quaternion() * FQuat(FVector::UpVector, FMath::DegreesToRadians(10))).Rotator();
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::TestRot %s %s"), *TestRot.ToString(), *Roll.Quaternion().ToString());

	//FRotator Rot = UKismetMathLibrary::MakeRotationFromAxes(FVector::DownVector, FVector::RightVector, FVector::ForwardVector);

	FVector OverrideUp = FVector::ForwardVector;
	FVector OverrideForward = FVector::DownVector;
	FVector OverrideRight = FVector::RightVector;
	
	FRotator FinalRot = (ViewTransform.GetRotation().Quaternion() * FQuat(OverrideForward, FMath::DegreesToRadians(InRot.Yaw))).Rotator();
	FinalRot = (FinalRot.Quaternion() * FQuat(FVector::RightVector, FMath::DegreesToRadians(InRot.Pitch))).Rotator();
	
	FRotator DeltaRot = FinalRot - ViewTransform.GetRotation();
	DeltaRot.Normalize();

	InViewportClient->PeformDefaultCameraMovement(InDrag, DeltaRot, InScale);

	//FinalRot = (ViewTransform.GetRotation().Quaternion() * FQuat(FVector::RightVector, FMath::DegreesToRadians(InRot.Pitch))).Rotator();
	//DeltaRot = FinalRot - ViewTransform.GetRotation();
	//DeltaRot.Normalize();
	//InViewportClient->PeformDefaultCameraMovement(InDrag, DeltaRot, InScale);

	
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
		FRotator Rot = UKismetMathLibrary::MakeRotationFromAxes(FVector::ForwardVector, FVector::UpVector, FVector::RightVector);

		WorldToGravityTransform = FQuat::FindBetweenNormals(FVector::UpVector, Rot.Quaternion().GetUpVector());
		GravityToWorldTransform = WorldToGravityTransform.Inverse();
		
		ViewportClient->SetViewRotation(Rot);
		return true;
	}
	if (Event == IE_Pressed && Key == EKeys::T)
	{
		FRotator Rot = UKismetMathLibrary::MakeRotationFromAxes(FVector::ForwardVector, -FVector::RightVector, FVector::DownVector);

		WorldToGravityTransform = FQuat::FindBetweenNormals(FVector::UpVector, Rot.Quaternion().GetUpVector());
		GravityToWorldTransform = WorldToGravityTransform.Inverse();
		
		ViewportClient->SetViewRotation(Rot);
		return true;

		ViewportClient->SetViewRotation(Rot);
		return true;
	}
	if (Event == IE_Pressed && Key == EKeys::Y)
	{
		bDisable = !bDisable;
		UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::bDisable %d"), bDisable);
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
