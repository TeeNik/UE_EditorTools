#include "SpotComponentVisualizer.h"
#include "EditorTools/WorldObjects/SpotComponent.h"

void FSpotComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (auto* SpotComponent = Cast<USpotComponent>(Component))
	{
		const FVector Location = SpotComponent->GetComponentLocation();
		DrawWireSphere(PDI, Location, FColor::Yellow, 20, 16, SDPG_World);

		const FMatrix Matrix = FScaleRotationTranslationMatrix(
			SpotComponent->GetComponentScale(),
			SpotComponent->GetComponentRotation(),
			SpotComponent->GetComponentLocation());

		FColor Color = FColor::Blue;
		const UTexture2D* Icon = nullptr;

#if WITH_EDITORONLY_DATA
		Color = SpotComponent->DEBUG_Color;
		Icon = SpotComponent->DEBUG_Icon;
#endif

		DrawDirectionalArrow(PDI, Matrix, Color, 50, 10, SDPG_World, 2);

		if (Icon)
		{
			PDI->DrawSprite(Location, 50, 50, Icon->GetResource(), FColor::White, SDPG_Foreground, 
				0, Icon->GetResource()->GetSizeX(), 0, Icon->GetResource()->GetSizeY(), SE_BLEND_Masked);
		}
	}
}
