#include "ExampleActor.h"
#include "ExampleOptionActor.h"

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

#if WITH_EDITOR
void AExampleActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AExampleActor, Type))
	{
		ChildActorClass.Class = nullptr;
		ChildActorComponent->SetChildActorClass(ChildActorClass);
	}
}
#endif

void AExampleActor::IsClassCompatible(FClassSelectorParams& Params)
{
	if (Params.Class)
	{
		if (AExampleOptionActor* View = Cast<AExampleOptionActor>(Params.Class->GetDefaultObject()))
		{
			Params.Result = View->GetType() == Type;
		}
	}
}
