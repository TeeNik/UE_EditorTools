#pragma once

#include "CoreMinimal.h"
#include "EditorTools/Utils/ClassSelector.h"
#include "GameFramework/Actor.h"
#include "ExampleActor.generated.h"

UCLASS()
class EDITORTOOLS_API AExampleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AExampleActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Example)
	FClassSelector ChildActorClass;

	UPROPERTY(EditDefaultsOnly)
	UChildActorComponent* ChildActorComponent;

};
