// Fill out your copyright notice in the Description page of Project Settings.

#include "Pyramid.h"
#include "CubeGameState.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Block.h"

// Sets default values
APyramid::APyramid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APyramid::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();
	APyramid::GeneratePyramid(7, 101);

	GameState = World->GetGameState<ACubeGameState>();
	GameState->SetPyramidReference(this);
}

void APyramid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APyramid::EnableFallForFloatingBlocks()
{
	for (TPair<FIntPoint, ABlock*> BlockData : PyramidCoordinates)
	{
		if (IsTheBlockFloating(BlockData.Key))
		{
			// A quick, more performant method checks whether the fall should fall at all, first. Then, we calculate
			// where should it fall to.
			FIntPoint TargetCoordinates = APyramid::FindTargetCoordinateForFallingBlock(BlockData.Key);
			if (TargetCoordinates != BlockData.Key)
			{
				PyramidCoordinates.Remove(BlockData.Key);
				BlockData.Value->SetBlockFall(PyramidBlockWorldPositions[TargetCoordinates], TargetCoordinates);
			}

		}
	}
}

bool APyramid::IsTheBlockFloating(FIntPoint BlockCoordinates)
{
	if (BlockCoordinates.Y == 1) return false;
	FIntPoint BottomRightBlockCoordinates = FIntPoint(BlockCoordinates.X, BlockCoordinates.Y - 1);
	FIntPoint BottomLeftBlockCoordinates = FIntPoint(BlockCoordinates.X + 1, BlockCoordinates.Y - 1);
	return (!PyramidCoordinates.Contains(BottomLeftBlockCoordinates) && !PyramidCoordinates.Contains(BottomRightBlockCoordinates));
}

FIntPoint APyramid::FindTargetCoordinateForFallingBlock(FIntPoint FallingBlockCoordinates)
{
	// Iterates downwards through the pyramid until it finds a suitable block to fall to.
	bool bHasFoundCoordinate = false;
	FIntPoint CurrentBaseCoordinates = FallingBlockCoordinates;
	while (!bHasFoundCoordinate)
	{
		if (CurrentBaseCoordinates.Y == 1) return CurrentBaseCoordinates;

		FIntPoint BottomRightBlockCoordinates = FIntPoint(CurrentBaseCoordinates.X, CurrentBaseCoordinates.Y - 1);
		bool bBottomRightBlockExists = PyramidCoordinates.Contains(BottomRightBlockCoordinates);

		FIntPoint BottomLeftBlockCoordinates = FIntPoint(CurrentBaseCoordinates.X + 1, CurrentBaseCoordinates.Y - 1);
		bool bBottomLeftBlockExists = PyramidCoordinates.Contains(BottomLeftBlockCoordinates);

		if (!bBottomLeftBlockExists && !bBottomRightBlockExists)
		{
			FIntPoint NextUnderneathBlockCoordinates = FIntPoint(CurrentBaseCoordinates.X + 1, FMath::Clamp<int>(CurrentBaseCoordinates.Y - 2, 1, PyramidSize));
			if (!PyramidCoordinates.Contains(NextUnderneathBlockCoordinates))
			{
				CurrentBaseCoordinates = NextUnderneathBlockCoordinates;
			}
			else
			{
				// If the block is to fall to another block with the same offset, it will always check first the top-right block.
				return (bBottomRightBlockExists ? BottomRightBlockCoordinates : BottomLeftBlockCoordinates);
			}
		}
		else
		{
			return CurrentBaseCoordinates;
		}
	}
	return FallingBlockCoordinates;
}

void APyramid::GeneratePyramid(int Height, float Padding)
{
	for (size_t i = Height; i > 0; i--)
	{
		APyramid::GeneratePyramidLevel(i, Padding, Height);
	}
	PyramidSize = Height;
	// This keysort allows for the fall check to go upwards, so there are no floating blocks afterwards.
	PyramidCoordinates.KeySort([](FIntPoint A, FIntPoint B) {
		return A.Y < B.Y;
		});
}

void APyramid::GeneratePyramidLevel(int Level, float Padding, int LevelAmount)
{
	int YCoordinate = LevelAmount - Level;
	FVector RowStartPosition = StartingPosition + FVector(Padding * YCoordinate * 1 / 2, 0, YCoordinate * Padding);
	for (int LevelRow = 1; LevelRow <= Level; LevelRow++)
	{
		int XCoordinate = LevelRow;
		FVector RowPosition = RowStartPosition + FVector(XCoordinate * Padding, 0, 0);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		FIntPoint blockCoordinates = FIntPoint(XCoordinate, YCoordinate + 1);
		if (World)
		{
			ABlock* newBlock = World->SpawnActor<ABlock>(BlockBP);
			PyramidCoordinates.Add(blockCoordinates, newBlock);
			PyramidBlockWorldPositions.Add(blockCoordinates, RowPosition);
			FLinearColor ColorToApply = PossibleCubeColors[FMath::RandRange(0, PossibleCubeColors.Num() - 1)];
			newBlock->SetCubeColor(ColorToApply);
			newBlock->SetActorLocation(RowPosition);
			newBlock->InitializeBlock(this, blockCoordinates);
			newBlock->SetActorLabel(blockCoordinates.ToString());
		}
	}
}

void APyramid::StartBlockCascadeDestruction(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare, APlayerState* PlayerState)
{
	ABlock* StartingCascadeBlock = PyramidCoordinates[TargetedBlockCoordinates];
	PyramidCoordinates.Remove(TargetedBlockCoordinates);
	StartingCascadeBlock->Destroy();

	// Doing Fibonacci directly in the main recursion should be more performant. As the first block was already destroyed, the sum is initiated at 1.
	int PointSum = APyramid::ContinueBlockDestructionCascade(TargetedBlockCoordinates, ColorToCompare, 1, 0, 1);

	GameState->AddToPlayerScore(PlayerState, PointSum);

	if (PyramidCoordinates.Num() == 0)
	{
		for (APlayerState* _PlayerState : GameState->PlayerArray)
		{
			((APlayerCharacter*)_PlayerState->GetPawn())->SetupClientSideMatchEnd();
		}
	}

	APyramid::EnableFallForFloatingBlocks();
}

void APyramid::AddCubeToPyramidCoordinates(ABlock* BlockToAdd, FIntPoint Coordinates)
{
	PyramidCoordinates.Add(Coordinates, BlockToAdd);
}

int APyramid::ContinueBlockDestructionCascade(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare, int LastFibonacciValue, int SecondLastFibonacciValue, int PartialSum)
{
	int currentSum = PartialSum;
	for (FIntPoint Direction : DirectionsToCheck)
	{
		FIntPoint NextBlockCoordinates = TargetedBlockCoordinates + Direction;
		ABlock** BlockTest = (PyramidCoordinates.Find(NextBlockCoordinates));
		if (BlockTest != NULL)
		{
			ABlock* Block = *BlockTest;
			if (Block->GetColor() == ColorToCompare)
			{
				PyramidCoordinates.Remove(NextBlockCoordinates);
				Block->Destroy();
				currentSum += LastFibonacciValue + SecondLastFibonacciValue;
				currentSum = APyramid::ContinueBlockDestructionCascade(NextBlockCoordinates, ColorToCompare, LastFibonacciValue + SecondLastFibonacciValue, LastFibonacciValue, currentSum);
			}
		}
	}
	return currentSum;
}


