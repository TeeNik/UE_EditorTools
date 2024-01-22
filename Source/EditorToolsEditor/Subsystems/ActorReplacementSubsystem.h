#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "ActorReplacementSubsystem.generated.h"


UCLASS()
class UActorReplacementSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void OnNewActorsDropped(const TArray<UObject*>& DroppedObjects, const TArray<AActor*>& DroppedActors);
	void ReplaceIActor(TSubclassOf<UObject> BlueprintGeneratedClass, AActor* ActorToReplace, const UClass* ClassToSpawn);

};
