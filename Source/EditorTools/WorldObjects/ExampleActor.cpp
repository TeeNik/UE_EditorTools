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

void AExampleActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ChildActorComponent->SetChildActorClass(ChildActorClass);
	ChildActorComponent->CreateChildActor();
}
