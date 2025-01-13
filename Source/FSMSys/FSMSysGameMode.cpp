// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSMSysGameMode.h"
#include "FSMSysCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFSMSysGameMode::AFSMSysGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
