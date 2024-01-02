#include "ActorReplacementSubsystem.h"
#include "ActorReplacementSettings.h"
#include "Editor.h"
#include "ActorFactories/ActorFactoryBlueprint.h"
#include "EditorTools/WorldObjects/ExampleActor.h"

void UActorReplacementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FEditorDelegates::OnNewActorsDropped.AddUObject(this, &UActorReplacementSubsystem::OnNewActorsDropped);
}

void UActorReplacementSubsystem::Deinitialize()
{
	FEditorDelegates::OnNewActorsDropped.RemoveAll(this);
}

void UActorReplacementSubsystem::OnNewActorsDropped(const TArray<UObject*>& DroppedObjects,	const TArray<AActor*>& DroppedActors)
{
	auto* Settings = GetDefault<UActorReplacementSettings>();
	for (int Idx = 0; Idx < DroppedObjects.Num(); Idx++)
	{
		UObject* Obj = DroppedObjects[Idx];
		if (const UBlueprint* Blueprint = Cast<UBlueprint>(Obj))
		{
			const UClass* Class = Blueprint->ParentClass;
			for (auto& Item : Settings->ActorReplacements)
			{
				if (Class->IsChildOf(Item.Key.LoadSynchronous()))
				{
					ReplaceIActor(Blueprint->GeneratedClass, DroppedActors[Idx], Item.Value.LoadSynchronous());
				}
			}
		}
	}
}

void UActorReplacementSubsystem::ReplaceIActor(TSubclassOf<UObject> BlueprintGeneratedClass, AActor* ActorToReplace, const UClass* ClassToSpawn)
{
	if (!GEditor || !ClassToSpawn)
	{
		return;
	}

	const FAssetData TargetAsset(ClassToSpawn);
	UActorFactory* Factory = GEditor->FindActorFactoryByClass(UActorFactoryBlueprint::StaticClass());

	TArray<AActor*> OutNewActors;
	GEditor->ReplaceActors(Factory, TargetAsset, { ActorToReplace }, &OutNewActors, false);

	for (int i = 0; i < OutNewActors.Num(); ++i)
	{
		if (AExampleActor* ExampleActor = Cast<AExampleActor>(OutNewActors[i]))
		{
			//InteractableActor->SetViewActorClass(Cast<UClass>(BlueprintGeneratedClass));
		}
	}
}
