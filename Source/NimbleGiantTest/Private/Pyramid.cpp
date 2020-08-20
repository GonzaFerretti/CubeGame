// Fill out your copyright notice in the Description page of Project Settings.

#include "Pyramid.h"
#include "Block.h"

// Sets default values
APyramid::APyramid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APyramid::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();
	APyramid::GenerateCubeMaterials();
	APyramid::GeneratePyramid(7, 115);
}

// Called every frame
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

void APyramid::GeneratePyramid(int Height, float Padding)
{
	UE_LOG(LogTemp, Warning, TEXT("Test"));
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
		FLinearColor ColorToApply = PossibleCubeColors[FMath::RandRange(0, PossibleCubeColors.Num() - 1)];
		UMaterialInstanceDynamic* MaterialToApply = *PossibleCubeMaterials.FindKey(ColorToApply);
		newBlock->SetTintedMaterial(MaterialToApply, ColorToApply);
		newBlock->SetActorLocation(RowPosition);
		newBlock->InitializeBlock(this, blockCoordinates);
		newBlock->SetActorLabel(blockCoordinates.ToString());
	}
}

void APyramid::ExecuteAttackOnBlock(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare)
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
				APyramid::ExecuteAttackOnBlock(NextBlockCoordinates, ColorToCompare);
				Block->Destroy();
			}
		}
	}
}


