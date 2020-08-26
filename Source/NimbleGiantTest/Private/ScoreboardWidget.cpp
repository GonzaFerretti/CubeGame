// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/WidgetTree.h"
#include "PlayerCharacter.h"
#include "Components/CanvasPanelSlot.h"

void UScoreboardWidget::UpdatePlayerScoreText(APlayerState* PlayerState, int Score, UTextBlock* PlayerScoreText)
{
	FString PlayerLabel = ((PlayerState->GetPlayerId() == OwnerPlayer->GetPlayerId()) ? "You" : "Player" + FString::FromInt(PlayerState->GetPlayerId())) + ": ";
	PlayerScoreText->SetText(FText::FromString(PlayerLabel + TEXT(" ") + FString::FromInt(Score)));
}

void UScoreboardWidget::InitializeScoreUI(TArray<APlayerState*> PlayerStates)
{
	int i = PlayerScores.Num();
	for (APlayerState* PlayerState : PlayerStates)
	{
		UTextBlock* PlayerScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(FString::FromInt(PlayerState->GetPlayerId())));
		UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
		RootWidget->AddChild(PlayerScoreText);
		UCanvasPanelSlot* PanelSlot = (UCanvasPanelSlot*)PlayerScoreText->Slot;
		PanelSlot->SetPosition(ScoreBoardPosition + FVector2D(0, Spacing * i));
		UScoreboardWidget::UpdatePlayerScoreText(PlayerState, 0, PlayerScoreText);
		PlayerScores.Add(PlayerState, PlayerScoreText);
		i++;
	}
}


void UScoreboardWidget::UpdateUI(TArray<APlayerState*> PlayerStates)
{
	if (PlayerScores.Num() != PlayerStates.Num())
	{
		TArray<APlayerState*> NewPlayers = PlayerStates;
		for (APlayerState* PlayerState : PlayerStates)
		{
			for (TPair<APlayerState*, UTextBlock*> PlayerScore : PlayerScores)
			{
				if (PlayerState->GetPlayerId() == PlayerScore.Key->GetPlayerId())
				{
					NewPlayers.Remove(PlayerState);
				}
			}
		}
		InitializeScoreUI(NewPlayers);
	}
	for (APlayerState* PlayerState : PlayerStates)
	{
		UScoreboardWidget::UpdatePlayerScoreText(PlayerState, PlayerState->GetScore(), PlayerScores[PlayerState]);
	}
}

void UScoreboardWidget::SetPlayerReference(APlayerState* PlayerState)
{
	OwnerPlayer = PlayerState;
}

bool UScoreboardWidget::IsPlayerReferenceNull()
{
	return (!OwnerPlayer);
}

void UScoreboardWidget::ShowGameOver()
{
	GameOverText->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 1)));
}
