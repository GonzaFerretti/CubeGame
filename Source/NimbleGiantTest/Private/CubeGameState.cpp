// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "Pyramid.h"

void ACubeGameState::SetPyramidReference(APyramid* Pyramid)
{
	CubePyramid = Pyramid;
}

void ACubeGameState::AddToPlayerScore(APlayerState* PlayerState, int ScoreToAdd)
{
	PlayerState->SetScore(PlayerState->GetScore() + ScoreToAdd);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::White, FString::FromInt(ScoreToAdd) + PlayerState->GetActorLabel());
}

void ACubeGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACubeGameState,CubePyramid);
}

APyramid* ACubeGameState::GetPyramid()
{
	return CubePyramid.Get();
}
