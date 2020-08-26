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
	UPROPERTY(EditAnywhere)
	float Spacing;
	UPROPERTY(EditAnywhere)
	float FontSize;
	UPROPERTY(EditAnywhere)
	FVector2D ScoreBoardPosition;
	APlayerState* OwnerPlayer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameOverText;
	bool bGameHasEnded;
	void UpdatePlayerScoreText(APlayerState* PlayerState, int Score, UTextBlock* PlayerScoreText);
	void UpdateOrCreateScoreUIElement(APlayerState* PlayerState, int ScoreBoardIndex, bool bShouldCreate);
	void UpdateAndShowFinalScoreBoard(TArray<APlayerState*> PlayerStates);

	UPROPERTY(VisibleAnywhere)
	TMap<APlayerState*, UTextBlock*> PlayerScores;

	public:
	void UpdateInMatchScoreBoard(APlayerState* PlayerState);
	void SetPlayerReference(APlayerState* PlayerState);
	bool IsPlayerReferenceNull();
	void ShowGameOver(TArray<APlayerState*> PlayerStates);
};
