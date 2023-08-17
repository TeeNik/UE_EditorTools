#pragma once

#include "CoreMinimal.h"
#include "EditorTools/Utils/ClassSelector.h"
#include "GameFramework/Actor.h"
#include "ExampleActor.generated.h"

UENUM(BlueprintType)
enum EExampleActorType
{
	Sphere,
	Cube,
	Cylinder
};

UCLASS()
class EDITORTOOLS_API AExampleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AExampleActor();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION(CallInEditor)
	void IsClassCompatible(FClassSelectorParams& Params);

	UPROPERTY(EditAnywhere, Category = Example, meta = (SelectorClass = "ExampleOptionActor", SelectorFunc = "IsClassCompatible"))
	FClassSelector ChildActorClass;

	UPROPERTY(EditAnywhere, Category = Example)
	EExampleActorType Type;

	UPROPERTY(EditDefaultsOnly)
	UChildActorComponent* ChildActorComponent;

};
