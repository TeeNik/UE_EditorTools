#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleOptionActor.generated.h"

UCLASS(Abstract)
class EDITORTOOLS_API AExampleOptionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AExampleOptionActor();

protected:
	virtual void BeginPlay() override;

};
