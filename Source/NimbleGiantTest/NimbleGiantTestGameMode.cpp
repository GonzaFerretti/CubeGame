// Copyright Epic Games, Inc. All Rights Reserved.

#include "NimbleGiantTestGameMode.h"
#include "NimbleGiantTestHUD.h"
#include "NimbleGiantTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANimbleGiantTestGameMode::ANimbleGiantTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANimbleGiantTestHUD::StaticClass();
}
