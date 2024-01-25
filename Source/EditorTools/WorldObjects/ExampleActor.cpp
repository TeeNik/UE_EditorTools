#include "ExampleActor.h"
#include "ExampleOptionActor.h"

AExampleActor::AExampleActor()
{
	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("ChildActor");
	ChildActorComponent->SetupAttachment(RootComponent);
}

void AExampleActor::SetChildActorClass(TObjectPtr<UClass> Class)
{
	if (Class->IsChildOf(AExampleOptionActor::StaticClass()))
	{
		ChildActorClass.Class = Class;
		ChildActorComponent->SetChildActorClass(ChildActorClass);
	}
}

//void AExampleActor::TextCameraMove()
//{
//	UE_LOG(LogTemp, Log, TEXT("Log TextCameraMove"));
//
//	if (GEditor)
//	{
//		if (FEditorViewportClient* Client = (FEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient()))
//		{
//			FVector Drag(0, 0, 10);
//			FRotator Rot(0, 0, 10);
//			Client->MoveViewportCamera(Drag, Rot);
//			UE_LOG(LogTemp, Log, TEXT("Found Client"));
//		}
//	}
//}

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
