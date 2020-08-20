// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NimbleGiantTestHUD.generated.h"

UCLASS()
class ANimbleGiantTestHUD : public AHUD
{
	GENERATED_BODY()

public:
	ANimbleGiantTestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

