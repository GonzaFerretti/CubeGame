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
	APyramid::GeneratePyramid(4, 150);
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
		//((ABlock*)BlockBP.Get())->GetBaseMaterial();
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
}

void APyramid::ExecuteAttackOnBlock(FIntVector TargetedBlockCoordinates, FLinearColor ColorToCompare)
{
	TArray<FIntVector> DirectionsToCheck = TArray<FIntVector>();

	FIntVector UpNeighborBlockCoordinate = TargetedBlockCoordinates + FIntVector(0,0, 1);
	DirectionsToCheck.Add(UpNeighborBlockCoordinate);

	FIntVector DownNeighborBlockCoordinate = TargetedBlockCoordinates + FIntVector(0, 0, -1);
	DirectionsToCheck.Add(DownNeighborBlockCoordinate);

	FIntVector LeftNeighborBlockCoordinate = TargetedBlockCoordinates + FIntVector(0, 1, 0);
	DirectionsToCheck.Add(LeftNeighborBlockCoordinate);

	FIntVector RightNeighborBlockCoordinate = TargetedBlockCoordinates + FIntVector(0, -1, 0);
	DirectionsToCheck.Add(RightNeighborBlockCoordinate);

	FIntVector ForwardNeighborBlockCoordinate = TargetedBlockCoordinates + FIntVector(1, 0, 0);
	DirectionsToCheck.Add(ForwardNeighborBlockCoordinate);

	FIntVector BackNeighborBlockCoordinate = TargetedBlockCoordinates + FIntVector(-1, 0, 0);
	DirectionsToCheck.Add(BackNeighborBlockCoordinate);

	for (FIntVector Direction : DirectionsToCheck)
	{
		ABlock** BlockTest = (PyramidCoordinates.Find(Direction));
		if (BlockTest != NULL)
		{
			ABlock* Block = *BlockTest;
			if (Block->GetColor() == ColorToCompare)
			{
				PyramidCoordinates.Remove(Direction);
				APyramid::ExecuteAttackOnBlock(Direction, ColorToCompare);
				Block->Destroy();
			}
		}
	}
}

void APyramid::GeneratePyramidLevel(int Level, float Padding, int LevelAmount)
{
	// The first level has 1 width, the second has 3, the third has 5, and so on. This is the odd numbers sequence, known by its closed form 2x-1
	int LevelSize = 2 * Level - 1;

	int ZCoordinate = LevelAmount - Level;
	FVector RowStartPosition = StartingPosition + FVector(0, 0, ZCoordinate * Padding);
	for (int LevelRow = 1; LevelRow <= LevelSize; LevelRow++)
	{
		int XCoordinate = LevelRow - Level;
		//FVector RowPosition = RowStartPosition + FVector(Padding* (LevelRow - 1), 0, 0) - FVector(Padding * (Level - 1),0,0);
		FVector RowPosition = RowStartPosition + FVector(XCoordinate * Padding, 0, 0);
		for (int LevelColumn = 1; LevelColumn <= LevelSize; LevelColumn++)
		{
			//FVector ColumnPosition = RowPosition + FVector(0, Padding * (LevelColumn - 1), 0) - FVector(0, Padding * (Level - 1), 0);
			int YCoordinate = LevelColumn - Level;
			FVector ColumnPosition = RowPosition + FVector(0, YCoordinate * Padding, 0);
			UE_LOG(LogTemp, Warning, TEXT("Column"));
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;

			FIntVector blockCoordinates = FIntVector(XCoordinate, YCoordinate, ZCoordinate);
			ABlock* newBlock = World->SpawnActor<ABlock>(BlockBP);
			PyramidCoordinates.Add(blockCoordinates, newBlock);
			FLinearColor ColorToApply = PossibleCubeColors[FMath::RandRange(0, PossibleCubeColors.Num() - 1)];
			UMaterialInstanceDynamic* MaterialToApply = *PossibleCubeMaterials.FindKey(ColorToApply);
			newBlock->SetTintedMaterial(MaterialToApply,ColorToApply);
			newBlock->SetActorLocation(ColumnPosition);
			newBlock->InitializeBlock(this, blockCoordinates);
			newBlock->SetActorLabel(blockCoordinates.ToString());
		}
	}
}

