// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomSplineComponent.h"
#include "CustomSplineActor.h"
#include "CustomSplineMetadata.h"

USplineMetadata* UCustomSplineComponent::GetSplinePointsMetadata()
{
	if (ACustomSplineActor* Actor = Cast<ACustomSplineActor>(GetOwner()))
	{
		return Actor->GetSplineMetadata();
	}
	return nullptr;
}

const USplineMetadata* UCustomSplineComponent::GetSplinePointsMetadata() const
{
	if (ACustomSplineActor* Actor = Cast<ACustomSplineActor>(GetOwner()))
	{
		return Actor->GetSplineMetadata();
	}
	return nullptr;
}

void UCustomSplineComponent::FixupPoints()
{
#if WITH_EDITORONLY_DATA
	if (GetSplinePointsMetadata())
	{
		const int32 NumPoints = GetNumberOfSplinePoints();
		GetSplinePointsMetadata()->Fixup(NumPoints, this);
	}
#endif
}

void UCustomSplineComponent::PostLoad()
{
	Super::PostLoad();
	FixupPoints();
}

void UCustomSplineComponent::PostDuplicate(bool bDuplicateForPie)
{
	Super::PostDuplicate(bDuplicateForPie);
	FixupPoints();
}

#if WITH_EDITOR
void UCustomSplineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FixupPoints();
}

void UCustomSplineComponent::PostEditImport()
{
	Super::PostEditImport();
	FixupPoints();
}
#endif
