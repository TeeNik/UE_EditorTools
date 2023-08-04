#pragma once

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "PropertyCustomizationHelpers.h"

class ClassSelectorCustomization
{
};

class FClassSelectorFilter : public IClassViewerFilter
{
public:
	TObjectPtr<UClass> BaseClass;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override;
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override;

protected:
	bool CheckClass(const UClass* InClass) const;
	bool CheckUnloadedClass(const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData) const;
	bool CheckClassFilterFunction(const UClass* Class, const TSharedPtr<IPropertyHandle> PropertyHandle, TObjectPtr<UObject> PropertyOwner) const;
};

class FClassSelectorCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> InPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> InPropertyHandle, class IDetailChildrenBuilder& Builder, IPropertyTypeCustomizationUtils& CustomizationUtils) override { }
	virtual bool ShouldInlineKey() const override { return true; }

protected:

	TSharedRef<SWidget> GenerateClassPicker();
	FText GetDisplayValueAsString() const;

	void InitContent(TSharedRef<IPropertyHandle> InPropertyHandle);
	void OnClassPicked(UClass* Class);
	void OnBrowseTo();
	void OnClear();
	void ImportValue();
	void ExportValue(UClass* InClass);

	void OnPropertyChanged();

	FClassViewerInitializationOptions ClassViewerOptions;
	TObjectPtr<UClass> SelectedClass;

	TSharedPtr<SComboButton> ComboButton;

	TObjectPtr<UObject> PropertyOwner;
	TSharedPtr<IPropertyHandle> PropertyHandle;

};