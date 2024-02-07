// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCameraRotEdMode.h"
#include "Selection.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "FCustomCameraRotEditorMode"

const FEditorModeID FCustomCameraRotEdMode::EM_CustomCameraRotEdModeId = TEXT("EM_CustomCameraRotEditorMode");

FCustomCameraRotEdMode::FCustomCameraRotEdMode()
{
	Info = FEditorModeInfo(FCustomCameraRotEdMode::EM_CustomCameraRotEdModeId,
		LOCTEXT("CustomCameraRotEditorModeName", "CustomCameraRotEditorMode"),
		FSlateIcon(),
		true);
}


FCustomCameraRotEdMode::~FCustomCameraRotEdMode()
{
}

bool FCustomCameraRotEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
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

bool FCustomCameraRotEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
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

void FCustomCameraRotEdMode::UpdateNewUpTransforms(const FVector& NewUp)
{
	WorldToNewUpTransform = FQuat::FindBetweenNormals(FVector::UpVector, NewUp);
	NewUpToWorldTransform = WorldToNewUpTransform.Inverse();
}

void FCustomCameraRotEdMode::Enter()
{
	FEdMode::Enter();

	UpdateNewUpTransforms(FVector::UpVector);
}

void FCustomCameraRotEdMode::Exit()
{
	FEdMode::Exit();
}

bool FCustomCameraRotEdMode::ReceivedFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
	UE_LOG(LogTemp, Log, TEXT("FMyCustomEdMode::ReceivedFocus"));
	return false;
}

#undef LOCTEXT_NAMESPACE
