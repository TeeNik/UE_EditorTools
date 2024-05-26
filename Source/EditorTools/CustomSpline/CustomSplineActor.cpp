// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplineActor.h"

// Sets default values
ACustomSplineActor::ACustomSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

