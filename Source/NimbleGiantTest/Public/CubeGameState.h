// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CubeGameState.generated.h"

class APyramid;
UCLASS()
class NIMBLEGIANTTEST_API ACubeGameState : public AGameState
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, Category = "Game Data", Replicated)
	TSoftObjectPtr<APyramid> CubePyramid;

	public:
	void SetPyramidReference(APyramid* Pyramid);
	void AddToPlayerScore(APlayerState* PlayerState, int ScoreToAdd);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void StartGameOverPhase();
	APyramid* GetPyramid();
};
