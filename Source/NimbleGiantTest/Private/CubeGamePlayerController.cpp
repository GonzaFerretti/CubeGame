// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGamePlayerController.h"
#include "ScoreboardWidget.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ACubeGamePlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
}

void ACubeGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	UWorld* World = GetWorld();
	if (World)
	{
		if ((ACubeGamePlayerController*)World->GetFirstPlayerController() != this) return;
		AGameStateBase* GameState = World->GetGameState();
		if (!ScoreBoardUI)
		{
			ScoreBoardUI = CreateWidget<UScoreboardWidget>(World, ScoreBoardUIBp);
			ScoreBoardUI->AddToViewport();
		}
		else if (GetPlayerState<APlayerState>())
		{
			if (ScoreBoardUI->IsPlayerReferenceNull())
			{
				ScoreBoardUI->SetPlayerReference(GetPlayerState<APlayerState>());
			}
			if (GameState)
			{
				ScoreBoardUI->UpdateUI(GameState->PlayerArray);
			}
		}
	}
}
