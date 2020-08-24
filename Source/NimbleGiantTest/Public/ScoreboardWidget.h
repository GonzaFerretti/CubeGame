// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
/**
 * 
 */
UCLASS()
class NIMBLEGIANTTEST_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
	protected:
	UPROPERTY(EditAnywhere)
	UCanvasPanel* Panel;

	UPROPERTY(VisibleAnywhere)
	TMap<APlayerState*, UTextBlock*> PlayersScores;

	public:
	void InitializeScoreUI(TArray<APlayerState*> PlayerStates);
};
