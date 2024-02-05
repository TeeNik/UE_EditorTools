// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomEdMode.h"

#include "Selection.h"
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

	
	if( InViewportClient->GetCurrentWidgetAxis() != EAxisList::None || bDisable || InRot.IsNearlyZero())
	{
		return false;
	}
		
	//UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::WidgetAxis %d"), (int)InViewportClient->GetCurrentWidgetAxis());
	//UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::CameraRot %s"), *InViewportClient->GetViewRotation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::InputDelta %s %s"), *InDrag.ToString(), *InRot.ToString());

	FRotator ViewRotation = InViewportClient->GetViewRotation();
	ViewRotation = (NewUpToWorldTransform * ViewRotation.Quaternion()).Rotator();
	ViewRotation += FRotator( InRot.Pitch, InRot.Yaw, InRot.Roll );
	// normalize to -180 to 180
	ViewRotation.Pitch = FRotator::NormalizeAxis(ViewRotation.Pitch);
	// Make sure its withing  +/- 90 degrees (minus a small tolerance to avoid numerical issues w/ camera orientation conversions later on).
	ViewRotation.Pitch = FMath::Clamp( ViewRotation.Pitch, -90.f+1, 90.f-1 );
	ViewRotation = (WorldToNewUpTransform * ViewRotation.Quaternion()).Rotator();

	
	FRotator OldViewRotation = InViewportClient->GetViewRotation();
	OldViewRotation += FRotator( InRot.Pitch, InRot.Yaw, InRot.Roll );
	// normalize to -180 to 180
	OldViewRotation.Pitch = FRotator::NormalizeAxis(OldViewRotation.Pitch);
	// Make sure its withing  +/- 90 degrees (minus a small tolerance to avoid numerical issues w/ camera orientation conversions later on).
	OldViewRotation.Pitch = FMath::Clamp( OldViewRotation.Pitch, -90.f+KINDA_SMALL_NUMBER, 90.f-KINDA_SMALL_NUMBER );

	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::ViewRotation %s"), *ViewRotation.ToString());
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::OldViewRotation %s"), *OldViewRotation.ToString());

	//return false;

	FRotator DeltaRot = ViewRotation - InViewportClient->GetViewRotation();
	DeltaRot.Normalize();
	InViewportClient->PeformDefaultCameraMovement(InDrag, DeltaRot, InScale);
	//InViewportClient->SetViewRotation(ViewRotation);
	
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

		WorldToNewUpTransform = FQuat::FindBetweenNormals(FVector::UpVector, Rot.Quaternion().GetUpVector());
		NewUpToWorldTransform = WorldToNewUpTransform.Inverse();
		
		ViewportClient->SetViewRotation(Rot);
		return true;
	}
	if (Event == IE_Pressed && Key == EKeys::T)
	{
		FRotator Rot = UKismetMathLibrary::MakeRotationFromAxes(FVector::ForwardVector, -FVector::RightVector, FVector::DownVector);

		WorldToNewUpTransform = FQuat::FindBetweenNormals(FVector::UpVector, Rot.Quaternion().GetUpVector());
		NewUpToWorldTransform = WorldToNewUpTransform.Inverse();
		
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
	if (Event == IE_Pressed && Key == EKeys::M)
	{
		TArray<UObject*> SelectedActors;
		if (GEditor->GetSelectedActorCount() > 0)
		{
			if (AActor* Actor = GEditor->GetSelectedActors()->GetTop<AActor>())
			{
				const FRotator Rot = UKismetMathLibrary::MakeRotFromZ(Actor->GetActorUpVector());

				WorldToNewUpTransform = FQuat::FindBetweenNormals(FVector::UpVector, Rot.Quaternion().GetUpVector());
				NewUpToWorldTransform = WorldToNewUpTransform.Inverse();
		
				ViewportClient->SetViewRotation(Rot);
				UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::SelectedActor %s"), *GetNameSafe(Actor));
			}
		}
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
