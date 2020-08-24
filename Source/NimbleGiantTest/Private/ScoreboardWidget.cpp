// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"

void UScoreboardWidget::InitializeScoreUI(TArray<APlayerState*> PlayerStates)
{
	int i = 0;
	for (APlayerState* PlayerState : PlayerStates)
	{
		//UTextBlock* PlayerScoreText = CreateWidget<UTextBlock>(GetWorld(), UTextBlock::StaticClass());
		FString PlayerLabel = PlayerState->GetActorLabel();
		UTextBlock* PlayerScoreText =  WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(PlayerLabel));
		PlayerScoreText->SetText(FText::FromString(PlayerLabel));
		Panel->InsertChildAt(i, PlayerScoreText);
		i++;
	}
}
