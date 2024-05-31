// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomSplineMetadataDetailsFactory.h"
#include "ComponentVisualizer.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"
#include "EditorTools/CustomSpline/CustomSplineMetadata.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "FCustomSplineMetadataDetails"

TSharedPtr<ISplineMetadataDetails> UCustomSplineMetadataDetailsFactory::Create()
{
	return MakeShared<FCustomSplineMetadataDetails>();
}

UClass* UCustomSplineMetadataDetailsFactory::GetMetadataClass() const
{
	return UCustomSplineMetadata::StaticClass();
}

FName FCustomSplineMetadataDetails::GetName() const
{
	return FName(TEXT("CustomSplineMetadataDetails"));
}

FText FCustomSplineMetadataDetails::GetDisplayName() const
{
	return LOCTEXT("FCustomSplineMetadataDetails", "EditorTools");
}

template<class T>
bool UpdateMultipleValue(TOptional<T>& CurrentValue, T InValue)
{
	if (!CurrentValue.IsSet())
	{
		CurrentValue = InValue;
	}
	else if (CurrentValue.IsSet() && CurrentValue.GetValue() != InValue)
	{
		CurrentValue.Reset();
		return false;
	}

	return true;
}

void FCustomSplineMetadataDetails::Update(USplineComponent* InSplineComponent, const TSet<int32>& InSelectedKeys)
{
	SplineComp = InSplineComponent;
	SelectedKeys = InSelectedKeys;
	TestFloatValue.Reset();

	if (InSplineComponent)
	{
		bool bUpdateTestFloat = true;

		UCustomSplineMetadata* Metadata = Cast<UCustomSplineMetadata>(InSplineComponent->GetSplinePointsMetadata());
		if (Metadata)
		{
			for (int32 Index : InSelectedKeys)
			{
				if (Metadata->PointParams.IsValidIndex(Index))
				{
					if (bUpdateTestFloat)
					{
						bUpdateTestFloat = UpdateMultipleValue(TestFloatValue, Metadata->PointParams[Index].TestFloat);
					}
				}
			}
		}
	}
}

void FCustomSplineMetadataDetails::GenerateChildContent(IDetailGroup& DetailGroup)
{
	DetailGroup.AddWidgetRow()
		.Visibility(EVisibility::Visible)
		.NameContent()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("TestFloat", "TestFloat"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
		]
	.ValueContent()
		.MinDesiredWidth(125.0f)
		.MaxDesiredWidth(125.0f)
		[
			SNew(SNumericEntryBox<float>)
			.Value(this, &FCustomSplineMetadataDetails::GetTestFloat)
		.AllowSpin(false)
		.MinValue(0.0f)
		.MaxValue(TOptional<float>())
		.MinSliderValue(0.0f)
		.MaxSliderValue(TOptional<float>()) // No upper limit
		.UndeterminedString(LOCTEXT("Multiple", "Multiple"))
		.OnValueCommitted(this, &FCustomSplineMetadataDetails::OnSetTestFloat)
		.Font(IDetailLayoutBuilder::GetDetailFont())
		];
}

UCustomSplineMetadata* FCustomSplineMetadataDetails::GetMetadata() const
{
	UCustomSplineMetadata* Metadata = SplineComp ? Cast<UCustomSplineMetadata>(SplineComp->GetSplinePointsMetadata()) : nullptr;
	return Metadata;
}

TOptional<float> FCustomSplineMetadataDetails::GetTestFloat() const
{
	return TestFloatValue;
}

void FCustomSplineMetadataDetails::OnSetTestFloat(float NewValue, ETextCommit::Type CommitInfo)
{
	if (UCustomSplineMetadata* Metadata = GetMetadata())
	{
		const FScopedTransaction Transaction(LOCTEXT("SetTestFloat", "Set spline point test float"));

		for (int32 Index : SelectedKeys)
		{
			Metadata->PointParams[Index].TestFloat = NewValue;
		}

		OnSetValues(*this);
	}
}

void FCustomSplineMetadataDetails::OnSetValues(FCustomSplineMetadataDetails& Details)
{
	Details.SplineComp->GetSplinePointsMetadata()->Modify();
	Details.SplineComp->UpdateSpline();
	Details.SplineComp->bSplineHasBeenEdited = true;
	static FProperty* SplineCurvesProperty = FindFProperty<FProperty>(USplineComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(USplineComponent, SplineCurves));
	FComponentVisualizer::NotifyPropertyModified(Details.SplineComp, SplineCurvesProperty);
	Details.Update(Details.SplineComp, Details.SelectedKeys);

	GEditor->RedrawLevelEditingViewports(true);
}

#undef LOCTEXT_NAMESPACE
