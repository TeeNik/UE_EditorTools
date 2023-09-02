#include "ClassSelectorCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Selection.h"
#include "EditorTools/Utils/ClassSelector.h"
#include "Widgets/Input/STextComboBox.h"

bool FClassSelectorFilter::IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
	const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	if (CheckClass(InClass))
	{
		TArray<UObject*> OuterObjects;
		InInitOptions.PropertyHandle->GetOuterObjects(OuterObjects);
		if (!OuterObjects.IsEmpty())
		{
			return CheckClassFilterFunction(InClass, InInitOptions.PropertyHandle, OuterObjects[0]);
		}
	}
	return false;
}

bool FClassSelectorFilter::IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
	const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
	TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	if (CheckUnloadedClass(InUnloadedClassData))
	{
		auto Name = InUnloadedClassData->GetClassPathName();
		FSoftClassPath ClassPath(Name.ToString());
		if (UClass* Class = ClassPath.TryLoadClass<UObject>())
		{
			TArray<UObject*> OuterObjects;
			InInitOptions.PropertyHandle->GetOuterObjects(OuterObjects);
			if (!OuterObjects.IsEmpty())
			{
				return CheckClassFilterFunction(Class, InInitOptions.PropertyHandle, OuterObjects[0]);
			}
			return false;
		}
	}
	return false;
}

bool FClassSelectorFilter::CheckClass(const UClass* InClass) const
{
	return InClass && (BaseClass == nullptr || InClass->IsChildOf(BaseClass))
		&& !InClass->HasAnyClassFlags(CLASS_Abstract)
		&& !InClass->GetName().StartsWith(TEXT("SKEL_"))
		&& !InClass->GetName().StartsWith(TEXT("REINST_"));
}

bool FClassSelectorFilter::CheckUnloadedClass(const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData) const
{
	return InUnloadedClassData->IsChildOf(BaseClass) && !InUnloadedClassData->HasAnyClassFlags(CLASS_Abstract) &&
		!InUnloadedClassData->GetClassName()->StartsWith(TEXT("SKEL_")) && !InUnloadedClassData->GetClassName()->StartsWith(TEXT("REINST_"));
}

bool FClassSelectorFilter::CheckClassFilterFunction(const UClass* Class,
	const TSharedPtr<IPropertyHandle> PropertyHandle, TObjectPtr<UObject> PropertyOwner) const
{
	FString FuncName = PropertyHandle->GetMetaData(TEXT("SelectorFunc"));
	if (auto* Function = PropertyOwner->FindFunction(FName(FuncName)))
	{
		FClassSelectorParams Params;
		Params.Class = Class;
		PropertyOwner->ProcessEvent(Function, &Params);
		return Params.Result;
	}
	return true;
}

TSharedRef<IPropertyTypeCustomization> FClassSelectorCustomization::MakeInstance()
{
	return MakeShareable(new FClassSelectorCustomization);
}

void FClassSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<SHorizontalBox> ContentBox = SNew(SHorizontalBox);

	InitContent(InPropertyHandle);

	ContentBox->AddSlot().MaxWidth(250.0f)
		[
			SAssignNew(ComboButton, SComboButton)
			.OnGetMenuContent(this, &FClassSelectorCustomization::GenerateClassPicker)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &FClassSelectorCustomization::GetDisplayValueAsString)
		.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		];

	ContentBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			PropertyCustomizationHelpers::MakeBrowseButton(FSimpleDelegate::CreateSP(this, &FClassSelectorCustomization::OnBrowseTo))
		];

	ContentBox->AddSlot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			PropertyCustomizationHelpers::MakeClearButton(FSimpleDelegate::CreateSP(this, &FClassSelectorCustomization::OnClear))
		];

	HeaderRow.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.VAlign(VAlign_Center)
		[
			ContentBox.ToSharedRef()
		];
}

void FClassSelectorCustomization::InitContent(TSharedRef<IPropertyHandle> InPropertyHandle)
{
	PropertyHandle = InPropertyHandle;
	TArray<UObject*> OuterObjects;
	PropertyHandle->GetOuterObjects(OuterObjects);
	if (!OuterObjects.IsEmpty())
	{
		PropertyOwner = OuterObjects[0];
	}

	if (TSharedPtr<IPropertyHandle> ParentHandle = PropertyHandle->GetParentHandle())
	{
		ParentHandle->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FClassSelectorCustomization::OnPropertyChanged));
	}

	ImportValue();

	ClassViewerOptions.ClassFilters.Reset();
	const TSharedRef<FClassSelectorFilter> ViewFilter = MakeShared<FClassSelectorFilter>();
	ViewFilter->BaseClass = PropertyHandle->GetClassMetaData(TEXT("SelectorClass"));
	ClassViewerOptions.ClassFilters.Add(ViewFilter);
	ClassViewerOptions.PropertyHandle = PropertyHandle;
	ClassViewerOptions.bShowNoneOption = true;
}

void FClassSelectorCustomization::OnClassPicked(UClass* InClass)
{
	ExportValue(InClass);
	ComboButton->SetIsOpen(false);
}

TSharedRef<SWidget> FClassSelectorCustomization::GenerateClassPicker()
{
	FOnClassPicked OnPicked(FOnClassPicked::CreateRaw(this, &FClassSelectorCustomization::OnClassPicked));

	return SNew(SBox)
		.WidthOverride(280)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(500)
		[
			FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(ClassViewerOptions, OnPicked)
		]
		];
}

FText FClassSelectorCustomization::GetDisplayValueAsString() const
{
	static bool bIsReentrant = false;
	static const FText None = FText::FromString(TEXT("None"));
	static const FText MultipleValues = FText::FromString(TEXT("MultipleValues"));

	if (!bIsReentrant)
	{
		TGuardValue<bool> Guard(bIsReentrant, true);

		if (PropertyHandleResult == FPropertyAccess::MultipleValues)
		{
			return MultipleValues;
		}

		if (SelectedClass)
		{
			return FText::FromString(SelectedClass.Get()->GetName());
		}
	}
	return None;
}

void FClassSelectorCustomization::OnBrowseTo()
{
	if (SelectedClass != NULL && SelectedClass->ClassGeneratedBy != NULL)
	{
		if (UBlueprint* Blueprint = Cast<UBlueprint>(SelectedClass->ClassGeneratedBy))
		{
			TArray<UObject*> SyncObjects;
			SyncObjects.Add(Blueprint);
			GEditor->SyncBrowserToObjects(SyncObjects);
		}
	}
}

void FClassSelectorCustomization::OnClear()
{
	ExportValue(nullptr);
}

void FClassSelectorCustomization::ImportValue()
{
	FString ValueStr;
	PropertyHandleResult = PropertyHandle->GetValueAsFormattedString(ValueStr, PPF_None);
	if (PropertyHandleResult == FPropertyAccess::Success)
	{
		FClassSelector StructVal;
		FClassSelector::StaticStruct()->ImportText(*ValueStr, &StructVal, nullptr, EPropertyPortFlags::PPF_None, nullptr, FClassSelector::StaticStruct()->GetName());
		SelectedClass = StructVal.Class;
	}
}

void FClassSelectorCustomization::ExportValue(UClass* InClass)
{
	SelectedClass = InClass;
	FString ValueStr;
	FClassSelector StructVal;
	StructVal.Class = InClass;
	FClassSelector::StaticStruct()->ExportText(ValueStr, &StructVal, &StructVal, nullptr, EPropertyPortFlags::PPF_None, nullptr);
	PropertyHandleResult = PropertyHandle->SetValueFromFormattedString(ValueStr);
}

void FClassSelectorCustomization::OnPropertyChanged()
{
	ImportValue();
}