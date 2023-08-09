// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleOptionActor.generated.h"

UCLASS()
class EDITORTOOLS_API AExampleOptionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AExampleOptionActor();

protected:
	virtual void BeginPlay() override;

};
