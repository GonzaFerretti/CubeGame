// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacter.h"
#include "Pyramid.h"

void ACubeGameState::SetPyramidReference(APyramid* Pyramid)
{
	CubePyramid = Pyramid;
}

void ACubeGameState::AddToPlayerScore(APlayerState* PlayerStateClientSide, int ScoreToAdd)
{
	APlayerState* PlayerStateServerSide  = (PlayerArray.FindByPredicate([PlayerStateClientSide](APlayerState* A) {
		return A->GetPlayerId() == PlayerStateClientSide->GetPlayerId();
		}))[0];
		PlayerStateServerSide->SetScore(PlayerStateServerSide->GetScore() + ScoreToAdd);
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
