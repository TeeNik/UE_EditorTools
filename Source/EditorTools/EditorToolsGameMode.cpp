// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorToolsGameMode.h"
#include "EditorToolsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEditorToolsGameMode::AEditorToolsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
