// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCameraRotationEdMode.h"
#include "Selection.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "FSampleToolsEditorMode"

const FEditorModeID FCustomCameraRotationEdMode::EM_MyCustomEditorModeId = TEXT("EM_MyCustomEditorMode");

FCustomCameraRotationEdMode::FCustomCameraRotationEdMode()
{
	Info = FEditorModeInfo(FCustomCameraRotationEdMode::EM_MyCustomEditorModeId,
		LOCTEXT("SMyCustomEditorModeName", "MyCustomEditorMode"),
		FSlateIcon(),
		true);
}


FCustomCameraRotationEdMode::~FCustomCameraRotationEdMode()
{
}

bool FCustomCameraRotationEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	if( InViewportClient->GetCurrentWidgetAxis() != EAxisList::None || InRot.IsNearlyZero())
	{
		return false;
	}

	FRotator ViewRotation = InViewportClient->GetViewRotation();
	ViewRotation = (NewUpToWorldTransform * ViewRotation.Quaternion()).Rotator();
	
	ViewRotation += FRotator( InRot.Pitch, InRot.Yaw, InRot.Roll );
	// normalize to -180 to 180
	ViewRotation.Pitch = FRotator::NormalizeAxis(ViewRotation.Pitch);
	// Make sure its withing  +/- 90 degrees (minus a small tolerance to avoid numerical issues w/ camera orientation conversions later on).
	ViewRotation.Pitch = FMath::Clamp( ViewRotation.Pitch, -90.f+1, 90.f-1 );
	
	ViewRotation = (WorldToNewUpTransform * ViewRotation.Quaternion()).Rotator();

	
	FRotator DeltaRot = ViewRotation - InViewportClient->GetViewRotation();
	InViewportClient->PeformDefaultCameraMovement(InDrag, DeltaRot, InScale);
	
	return true;
}

bool FCustomCameraRotationEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Event == IE_Pressed && Key == EKeys::M)
	{
		TArray<UObject*> SelectedActors;
		if (GEditor->GetSelectedActorCount() > 0)
		{
			if (AActor* Actor = GEditor->GetSelectedActors()->GetTop<AActor>())
			{
				UpdateNewUpTransforms(Actor->GetActorUpVector());
				const FRotator Rot = UKismetMathLibrary::MakeRotFromZ(Actor->GetActorUpVector());
				ViewportClient->SetViewRotation(Rot);
			}
		}
		return true;
	}
	return false;
}

void FCustomCameraRotationEdMode::UpdateNewUpTransforms(const FVector& NewUp)
{
	WorldToNewUpTransform = FQuat::FindBetweenNormals(FVector::UpVector, NewUp);
	NewUpToWorldTransform = WorldToNewUpTransform.Inverse();
}

void FCustomCameraRotationEdMode::Enter()
{
	FEdMode::Enter();

	UpdateNewUpTransforms(FVector::UpVector);
}

void FCustomCameraRotationEdMode::Exit()
{
	FEdMode::Exit();
}

bool FCustomCameraRotationEdMode::ReceivedFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::ReceivedFocus"));
	return false;
}

#undef LOCTEXT_NAMESPACE
