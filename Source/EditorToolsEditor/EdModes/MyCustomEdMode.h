// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class EDITORTOOLSEDITOR_API FMyCustomEdMode : public FEdMode, public ILegacyEdModeViewportInterface
{
public:
	const static FEditorModeID EM_MyCustomEditorModeId;

	FMyCustomEdMode();
	virtual ~FMyCustomEdMode();
	
	virtual void Enter() override;
	virtual void ActorSelectionChangeNotify() override;
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;

	
};
