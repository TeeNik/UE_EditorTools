#pragma once
#include "ComponentVisualizer.h"


class FSpotComponentVisualizer : public FComponentVisualizer
{
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
