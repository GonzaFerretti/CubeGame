// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeGameState.h"
#include "Net/UnrealNetwork.h"
#include "Pyramid.h"

void ACubeGameState::SetPyramid(APyramid* Pyramid)
{
	CubePyramid = Pyramid;
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
