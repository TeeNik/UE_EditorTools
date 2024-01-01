#pragma once

#include "CoreMinimal.h"
#include "ActorReplacementSettings.generated.h"


UCLASS(Config = Editor, DefaultConfig)
class EDITORTOOLS_API UActorReplacementSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, meta = (AllowAbstract = true))
	TMap<TSoftClassPtr<AActor>, TSoftClassPtr<AActor>> ActorReplacements;
	
};
