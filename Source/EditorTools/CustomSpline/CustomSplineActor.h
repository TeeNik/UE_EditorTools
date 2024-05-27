// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomSplineActor.generated.h"

class UCustomSplineComponent;
class UCustomSplineMetadata;

UCLASS()
class EDITORTOOLS_API ACustomSplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomSplineActor();
	UCustomSplineMetadata* GetSplineMetadata() const;

private:
	UPROPERTY(Instanced, Export)
	UCustomSplineMetadata* SplineMetadata = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCustomSplineComponent* SplineComponent = nullptr;

};
