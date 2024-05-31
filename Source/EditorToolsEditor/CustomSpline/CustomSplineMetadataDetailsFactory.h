// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplineMetadataDetailsFactory.h"
#include "CustomSplineMetadataDetailsFactory.generated.h"


UCLASS()
class EDITORTOOLSEDITOR_API UCustomSplineMetadataDetailsFactory : public USplineMetadataDetailsFactoryBase
{
	GENERATED_BODY()

	virtual ~UCustomSplineMetadataDetailsFactory() override {}
	virtual TSharedPtr<ISplineMetadataDetails> Create() override;
	virtual UClass* GetMetadataClass() const override;
	
};

class EDITORTOOLSEDITOR_API FCustomSplineMetadataDetails : public ISplineMetadataDetails, public TSharedFromThis<FCustomSplineMetadataDetails>
{
public:

	virtual ~FCustomSplineMetadataDetails() {}
	virtual FName GetName() const override;
	virtual FText GetDisplayName() const override;
	virtual void Update(USplineComponent* InSplineComponent, const TSet<int32>& InSelectedKeys) override;
	virtual void GenerateChildContent(IDetailGroup& DetailGroup) override;

private:
	UCustomSplineMetadata* GetMetadata() const;
	TOptional<float> GetTestFloat() const;
	void OnSetTestFloat(float NewValue, ETextCommit::Type CommitInfo);
	void OnSetValues(FCustomSplineMetadataDetails& Details);

	TOptional<float> TestFloatValue;
	USplineComponent* SplineComp = nullptr;
	TSet<int32> SelectedKeys;
};
