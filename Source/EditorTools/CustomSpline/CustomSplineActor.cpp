// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplineActor.h"
#include "CustomSplineComponent.h"
#include "CustomSplineMetadata.h"

ACustomSplineActor::ACustomSplineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineMetadata = CreateDefaultSubobject<UCustomSplineMetadata>(TEXT("CustomSplineMetadata"));
	SplineMetadata->Reset(2);
	SplineMetadata->AddPoint(0.0f);
	SplineMetadata->AddPoint(1.0f);

	SplineComponent = CreateDefaultSubobject<UCustomSplineComponent>(TEXT("CustomSplineComponen"));
	SetRootComponent(SplineComponent);
}

UCustomSplineMetadata* ACustomSplineActor::GetSplineMetadata() const
{
	return SplineMetadata;
}

float ACustomSplineActor::GetTestFloatAtSplinePoint(int32 PointIndex) const
{
	if (ensure(SplineMetadata) && ensure(SplineMetadata->PointParams.IsValidIndex(PointIndex)))
	{
		return SplineMetadata->PointParams[PointIndex].TestFloat;
	}
	return 0.0f;
}
