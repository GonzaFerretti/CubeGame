// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGamePlayerController.h"
#include "ScoreboardWidget.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ACubeGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	UpdateScoreBoard();
}

void ACubeGamePlayerController::UpdateScoreBoard()
{
	// This method makes sure every part of the UI is initiated in proper order.
	UWorld* World = GetWorld();
	if (World)
	{
		// This check makes sure the other controllers in the listen server client aren't trying to create aditional Widgets.
		if ((ACubeGamePlayerController*)World->GetFirstPlayerController() != this) return;
		AGameStateBase* GameState = World->GetGameState();
		APlayerState* _PlayerState = GetPlayerState<APlayerState>();
		if (!ScoreBoardUI)
		{
			ScoreBoardUI = CreateWidget<UScoreboardWidget>(World, ScoreBoardUIBp);
			ScoreBoardUI->AddToViewport();
		}
		else if (_PlayerState)
		{
			if (ScoreBoardUI->IsPlayerReferenceNull())
			{
				ScoreBoardUI->SetPlayerReference(_PlayerState);
			}
			if (GameState)
			{
				ScoreBoardUI->UpdateInMatchScoreBoard(_PlayerState);
			}
		}
	}
}
