#include "ExampleActor.h"

AExampleActor::AExampleActor()
{
	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("ChildActor");
	ChildActorComponent->SetupAttachment(RootComponent);
}

void AExampleActor::BeginPlay()
{
	Super::BeginPlay();
	
}