// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void UScoreboardWidget::InitializeScoreUI(TArray<APlayerState*> PlayerStates)
{
	int i = 0;
	for (APlayerState* PlayerState : PlayerStates)
	{
		FString PlayerLabel = PlayerState->GetActorLabel();
		UTextBlock* PlayerScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(PlayerLabel));
		PlayerScoreText->SetText(FText::FromString(PlayerLabel + TEXT(" ") + FString::FromInt(0)));
		UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
		RootWidget->AddChild(PlayerScoreText);
		UCanvasPanelSlot* PanelSlot = (UCanvasPanelSlot*)PlayerScoreText->Slot;
		PanelSlot->SetPosition(FVector2D(0, Spacing * i));
		PlayersScores.Add(PlayerState, PlayerScoreText);
		i++;
	}
}

void UScoreboardWidget::UpdateUI(TArray<APlayerState*> PlayerStates)
{
	if (PlayersScores.Num() != PlayerStates.Num())
	{
		for (TPair<APlayerState*, UTextBlock*> PlayerState : PlayersScores)
		{
			UTextBlock* TextToCleanup;
			PlayersScores.RemoveAndCopyValue(PlayerState.Key,TextToCleanup);
			TextToCleanup->RemoveFromParent();
		}
		InitializeScoreUI(PlayerStates);
	}
	for (APlayerState* PlayerState : PlayerStates)
	{
		FString PlayerLabel = PlayerState->GetActorLabel();
		int PlayerScore = PlayerState->GetScore();
		PlayersScores[PlayerState]->SetText(FText::FromString(PlayerLabel + TEXT(" ") + FString::FromInt(PlayerScore)));
	}
}
