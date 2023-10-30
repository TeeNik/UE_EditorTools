// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpotComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDITORTOOLS_API USpotComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USpotComponent();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Spot", meta = (DisplayName = "Color"))
	FColor DEBUG_Color = FColor::Blue;

	UPROPERTY(EditAnywhere, Category = "Spot", meta = (DisplayName = "Icon"))
	TObjectPtr<UTexture2D> DEBUG_Icon;
#endif

		
};
