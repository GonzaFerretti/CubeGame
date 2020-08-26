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

void UScoreboardWidget::UpdateOrCreateScoreUIElement(APlayerState* PlayerState, int ScoreBoardIndex, bool bShouldCreate)
{
	UTextBlock* PlayerScoreText;
	if (bShouldCreate)
	{
		PlayerScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(FString::FromInt(PlayerState->GetPlayerId())));
		UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
		RootWidget->AddChild(PlayerScoreText);
		PlayerScores.Add(PlayerState, PlayerScoreText);
	}

	PlayerScoreText = PlayerScores[PlayerState];
	UCanvasPanelSlot* PanelSlot = (UCanvasPanelSlot*)PlayerScoreText->Slot;
	PanelSlot->SetPosition(ScoreBoardPosition + FVector2D(0, Spacing * ScoreBoardIndex));
	UpdatePlayerScoreText(PlayerState, PlayerState->GetScore(), PlayerScoreText);
}


void UScoreboardWidget::UpdateAndShowFinalScoreBoard(TArray<APlayerState*> PlayerStates)
{
	Algo::Sort(PlayerStates, ([](APlayerState* A, APlayerState* B) {
		return A->GetScore() >= B->GetScore();
		}));
	for (size_t i = 0; i < PlayerStates.Num(); i++)
	{
		APlayerState* PlayerState = PlayerStates[i];
		bool bIsOwnerPlayer = PlayerState->GetPlayerId() != OwnerPlayer->GetPlayerId();
		UpdateOrCreateScoreUIElement(PlayerState, i, bIsOwnerPlayer);
	}
}

void UScoreboardWidget::UpdateInMatchScoreBoard(APlayerState* PlayerState)
{
	if (!bGameHasEnded)
	{
		bool bThisPlayerScoreWasCreated = PlayerScores.Contains(PlayerState);
		UScoreboardWidget::UpdateOrCreateScoreUIElement(PlayerState, 0, !bThisPlayerScoreWasCreated);
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

void UScoreboardWidget::ShowGameOver(TArray<APlayerState*> PlayerStates)
{
	GameOverText->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 1)));
	bGameHasEnded = true;
	UScoreboardWidget::UpdateAndShowFinalScoreBoard(PlayerStates);
}
