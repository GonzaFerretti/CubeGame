// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubeGamePlayerController.generated.h"

/**
 * 
 */
class UScoreboardWidget;

UCLASS()
class NIMBLEGIANTTEST_API ACubeGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	protected:
	void PlayerTick(float DeltaTime) override;

	void UpdateScoreBoard();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UScoreboardWidget> ScoreBoardUIBp;

	public:
	UScoreboardWidget* ScoreBoardUI;
};
