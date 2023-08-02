#pragma once
#include "CoreMinimal.h"
#include "ClassSelector.generated.h"

USTRUCT()
struct FClassSelectorParams
{
	GENERATED_BODY()

	UPROPERTY()
	const UClass* Class = nullptr;

	UPROPERTY()
	bool Result = false;
};

// FClassSelector supports several meta specifiers:
// SelectorClass - filter will show only children of SelectorClass
// SelectorFunc - Function(FClassSelectorParams&) with name SelectorFunc will be called on property owner

USTRUCT(BlueprintType)
struct FClassSelector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UClass> Class;

	FClassSelector() {}

	FClassSelector(TObjectPtr<UClass> InClass) : Class(InClass)
	{
	}

	operator UClass* () const
	{
		return Class.Get();
	}
};