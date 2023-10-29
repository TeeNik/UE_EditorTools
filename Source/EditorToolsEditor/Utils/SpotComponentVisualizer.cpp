#include "SpotComponentVisualizer.h"
#include "EditorTools/WorldObjects/SpotComponent.h"

void FSpotComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (auto* SpotComponent = Cast<USpotComponent>(Component))
	{
		const FVector Location = SpotComponent->GetComponentLocation();
		DrawWireSphere(PDI, Location, FColor::Yellow, 20, 16, SDPG_World);
	}
}
