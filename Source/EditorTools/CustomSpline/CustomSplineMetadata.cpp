// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplineMetadata.h"

void UCustomSplineMetadata::InsertPoint(int32 Index, float t, bool bClosedLoop)
{
	if (Index >= PointParams.Num())
	{
		AddPoint(static_cast<float>(Index));
	} else
	{
		PointParams.Insert(FCustomSplinePointParams{}, Index);
	}
	
	Modify();
}

void UCustomSplineMetadata::UpdatePoint(int32 Index, float t, bool bClosedLoop)
{
	Modify();
}

void UCustomSplineMetadata::AddPoint(float InputKey)
{
	PointParams.Add(FCustomSplinePointParams{});
	Modify();
}

void UCustomSplineMetadata::RemovePoint(int32 Index)
{
	PointParams.RemoveAt(Index);
	Modify();
}

void UCustomSplineMetadata::DuplicatePoint(int32 Index)
{
	FCustomSplinePointParams NewVal = PointParams[Index];
	PointParams.Insert(NewVal, Index);
	Modify();
}

void UCustomSplineMetadata::CopyPoint(const USplineMetadata* FromSplineMetadata, int32 FromIndex, int32 ToIndex)
{
	if (const UCustomSplineMetadata* FromMetadata = Cast<UCustomSplineMetadata>(FromSplineMetadata))
	{
		PointParams[ToIndex] = PointParams[FromIndex];
		Modify();
	}
}

void UCustomSplineMetadata::Reset(int32 NumPoints)
{
	PointParams.Reset(NumPoints);
	Modify();
}

void UCustomSplineMetadata::Fixup(int32 NumPoints, USplineComponent* SplineComp)
{
	if (PointParams.Num() > NumPoints)
	{
		PointParams.RemoveAt(NumPoints, PointParams.Num() - NumPoints);
		Modify();
	}

	while (PointParams.Num() < NumPoints)
	{
		PointParams.Add(FCustomSplinePointParams{});
		Modify();
	}
}
