#pragma once

#include "CoreMinimal.h"
#include "ExampleActor.h"
#include "GameFramework/Actor.h"
#include "ExampleOptionActor.generated.h"

UCLASS(Abstract)
class EDITORTOOLS_API AExampleOptionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AExampleOptionActor();

	EExampleActorType GetType() const { return Type; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Example")
	EExampleActorType Type;

};
