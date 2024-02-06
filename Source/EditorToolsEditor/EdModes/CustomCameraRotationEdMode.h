// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class EDITORTOOLSEDITOR_API FCustomCameraRotationEdMode : public FEdMode, public ILegacyEdModeViewportInterface
{
public:
	const static FEditorModeID EM_MyCustomEditorModeId;

	FCustomCameraRotationEdMode();
	virtual ~FCustomCameraRotationEdMode();

protected:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual bool ReceivedFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport) override;

	
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;

	void UpdateNewUpTransforms(const FVector& NewUp);
	
	FQuat WorldToNewUpTransform;
	FQuat NewUpToWorldTransform;
};
