// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "CustomSplineComponent.generated.h"


USTRUCT()
struct FCustomSplinePointParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TestFloat = 1.0f;
	
};

UCLASS(meta=(BlueprintSpawnableComponent))
class EDITORTOOLS_API UCustomSplineComponent : public USplineComponent
{
	GENERATED_BODY()

public:	
	virtual USplineMetadata* GetSplinePointsMetadata() override;
	virtual const USplineMetadata* GetSplinePointsMetadata() const override;
	virtual void PostLoad() override;
	virtual void PostDuplicate(bool bDuplicateForPie) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditImport() override;
#endif
	void FixupPoints();
		
};
