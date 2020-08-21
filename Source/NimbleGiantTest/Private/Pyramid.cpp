// Fill out your copyright notice in the Description page of Project Settings.

#include "Pyramid.h"
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
	APyramid::GenerateCubeMaterials();
	APyramid::GeneratePyramid(7, 101);
}

void APyramid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APyramid::GenerateCubeMaterials()
{
	for (FLinearColor PossibleColor : PossibleCubeColors)
	{
		UMaterialInstanceDynamic* TintedMaterial = UMaterialInstanceDynamic::Create(BaseCubeMaterial, this, FName(PossibleColor.ToString()));
		TintedMaterial->SetVectorParameterValue("TintColor", PossibleColor);
		PossibleCubeMaterials.Add(TintedMaterial, PossibleColor);
	}
}

void APyramid::EnableFallForFloatingBlocks()
{
	// Sorting the coordinates by its level allows me to remove the falling blocks on the lower levels first.
	PyramidCoordinates.KeySort([](FIntPoint A, FIntPoint B) {
		return A.Y < B.Y;
		});
	for (TPair<FIntPoint, ABlock*> BlockData : PyramidCoordinates)
	{
		if (IsTheBlockFloating(BlockData.Key))
		{

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
			FIntPoint NextUnderneathBlockCoordinates = FIntPoint(CurrentBaseCoordinates.X + 1, FMath::Clamp<int>(CurrentBaseCoordinates.Y -2,1, PyramidSize));
			if (!PyramidCoordinates.Contains(NextUnderneathBlockCoordinates))
			{
				CurrentBaseCoordinates = NextUnderneathBlockCoordinates;
			}
			else
			{
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
		ABlock* newBlock = World->SpawnActor<ABlock>(BlockBP);
		PyramidCoordinates.Add(blockCoordinates, newBlock);
		PyramidBlockWorldPositions.Add(blockCoordinates, RowPosition);
		FLinearColor ColorToApply = PossibleCubeColors[FMath::RandRange(0, PossibleCubeColors.Num() - 1)];
		UMaterialInstanceDynamic* MaterialToApply = *PossibleCubeMaterials.FindKey(ColorToApply);
		newBlock->SetTintedMaterial(MaterialToApply, ColorToApply);
		newBlock->SetActorLocation(RowPosition);
		newBlock->InitializeBlock(this, blockCoordinates);
		newBlock->SetActorLabel(blockCoordinates.ToString());
	}
}

void APyramid::StartBlockCascadeDestruction(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare)
{
	APyramid::ContinueBlockDestructionCascade(TargetedBlockCoordinates, ColorToCompare);
	APyramid::EnableFallForFloatingBlocks();
}

void APyramid::AddCubeToPyramidCoordinates(ABlock* BlockToAdd, FIntPoint Coordinates)
{
	PyramidCoordinates.Add(Coordinates, BlockToAdd);
}

void APyramid::ContinueBlockDestructionCascade(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare)
{
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
				APyramid::ContinueBlockDestructionCascade(NextBlockCoordinates, ColorToCompare);
				Block->Destroy();
			}
		}
	}
}


